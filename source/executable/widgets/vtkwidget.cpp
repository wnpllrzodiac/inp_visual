#include "vtkwidget.h"
#include "inpreader.h"
#include "pickinteractorstyle.h"
#include "utils/logging.h"
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QToolBar>
#include <QToolTip>
#include <QVBoxLayout>
#include <QVTKOpenGLNativeWidget.h>
#include <QWidget>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleRubberBand2D.h>
#include <vtkOutputWindow.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkThreshold.h>
#include <vtkThresholdPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>

// 自定义VTK输出窗口类
class CustomVtkOutputWindow : public vtkOutputWindow
{
public:
    static CustomVtkOutputWindow *New();
    vtkTypeMacro(CustomVtkOutputWindow, vtkOutputWindow);

    void DisplayText(const char *txt) override { Logging::info("[VTK] {}", txt); }
    void DisplayErrorText(const char *txt) override { Logging::error("[VTK] {}", txt); }
    void DisplayWarningText(const char *txt) override { Logging::warn("[VTK] {}", txt); }
    void DisplayGenericWarningText(const char *txt) override { Logging::warn("[VTK] {}", txt); }
};
vtkStandardNewMacro(CustomVtkOutputWindow);

VtkWidget::VtkWidget(QWidget *parent)
: QWidget(parent)
, emitter_(new SignalEmitter(this))
, current_style_(InteractorStyle::PickCell) // 初始化为PickCell
{
    vtkOutputWindow::SetInstance(CustomVtkOutputWindow::New());
    initUi();
    setupConnections();
}

void VtkWidget::initUi()
{
    // 创建工具栏
    ui_.toolbar = new QToolBar(this);
    ui_.switch_style = new QAction("Switch Style", this);
    ui_.toolbar->addAction(ui_.switch_style);

    ui_.tooltip_widget = new QLabel(this);
    ui_.tooltip_widget->setWindowFlags(Qt::ToolTip);
    ui_.tooltip_widget->setWindowOpacity(0.8);
    ui_.tooltip_widget->adjustSize();
    ui_.tooltip_widget->setStyleSheet("background-color: white; color: black;");

    // 创建VTK OpenGL部件
    render_window_ = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    render_window_->AddRenderer(renderer_);

    vtk_widget_ = new QVTKOpenGLNativeWidget(this);
    vtk_widget_->setRenderWindow(render_window_);

    renderer_->SetBackground(0.2, 0.3, 0.4);

    // 设置2D视角
    vtkCamera *camera = renderer_->GetActiveCamera();
    camera->ParallelProjectionOn(); // 使用平行投影
    camera->SetPosition(0, 0, 1);   // 设置相机位置
    camera->SetFocalPoint(0, 0, 0); // 设置焦点
    camera->SetViewUp(0, 1, 0);     // 设置向上方向

    // 使用自定义的2D拾取交互样式
    vtkRenderWindowInteractor *interactor = render_window_->GetInteractor();
    auto style = vtkSmartPointer<CellPickInteractorStyle>::New();
    style->setEmitter(emitter_);
    style->SetDefaultRenderer(renderer_);
    interactor->SetInteractorStyle(style);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(ui_.toolbar); // 将工具栏添加到布局中
    layout->addWidget(vtk_widget_);
    layout->setContentsMargins(0, 0, 0, 0);
}

void VtkWidget::setupConnections()
{
    connect(emitter_, &SignalEmitter::cellSelected, this, &VtkWidget::onCellSelected);
    connect(emitter_, &SignalEmitter::pointSelected, this, &VtkWidget::onPointSelected);
    connect(ui_.switch_style, &QAction::triggered, this, &VtkWidget::switchInteractorStyle);
}

void VtkWidget::switchInteractorStyle()
{
    vtkRenderWindowInteractor *interactor = render_window_->GetInteractor();
    if (current_style_ == InteractorStyle::PickCell)
    {
        // 切换到PickPoint模式
        auto style = vtkSmartPointer<PointPickInteractorStyle>::New();
        style->setEmitter(emitter_);
        style->SetDefaultRenderer(renderer_);
        interactor->SetInteractorStyle(style);
        current_style_ = InteractorStyle::PickPoint;
        Logging::info("Switched to PickPoint style.");
    }
    else
    {
        // 切换到PickCell模式
        auto style = vtkSmartPointer<CellPickInteractorStyle>::New();
        style->setEmitter(emitter_);
        style->SetDefaultRenderer(renderer_);
        interactor->SetInteractorStyle(style);
        current_style_ = InteractorStyle::PickCell;
        Logging::info("Switched to PickCell style.");
    }
}

vtkRenderer *VtkWidget::renderer() const { return renderer_.Get(); }

void VtkWidget::resetCamera() { renderer_->ResetCamera(); }

void VtkWidget::loadInpFile(const QString &filename, const FileType &type)
{
    // 清空当前场景
    renderer_->RemoveAllViewProps();

    // 创建解析器并读取文件
    InpReader reader;
    grid_ = vtkSmartPointer<vtkUnstructuredGrid>::New();
    if (type == FileType::Abaqus)
    {
        reader.parseAbaqus(filename);
        grid_ = reader.getMainGrid();
    }
    else if (type == FileType::Gmsh)
    {
        reader.parseGmsh(filename);
        grid_ = reader.getMainGrid();
    }

    auto node_sets = reader.getNodeSets();
    auto elset_sets = reader.getElementSets();
    if (grid_ && grid_->GetNumberOfPoints() > 0)
    {
        // 创建边缘Actor
        auto edgeMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        edgeMapper->SetInputData(grid_);

        auto edgeActor = vtkSmartPointer<vtkActor>::New();
        edgeActor->SetMapper(edgeMapper);
        edgeActor->GetProperty()->EdgeVisibilityOn();
        edgeActor->GetProperty()->SetEdgeColor(0.2, 0.2, 0.2);
        edgeActor->GetProperty()->SetOpacity(0.5);

        // 创建顶点Actor
        auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
        vertexFilter->SetInputData(grid_);
        vertexFilter->Update();

        auto pointMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        pointMapper->SetInputConnection(vertexFilter->GetOutputPort());

        auto pointActor = vtkSmartPointer<vtkActor>::New();
        pointActor->SetMapper(pointMapper);
        pointActor->GetProperty()->SetPointSize(2);   // 设置点的大小
        pointActor->GetProperty()->SetColor(1, 0, 0); // 设置点的颜色为红色

        // 添加到渲染器
        renderer_->AddActor(edgeActor);
        renderer_->AddActor(pointActor);
        renderer_->ResetCamera();
        render_window_->Render();
    }
    else
    {
        Logging::error("Failed to load or parse INP file: {}", filename);
    }
}

void VtkWidget::openAbaqusFile(const QString &filename)
{
    if (!filename.isEmpty())
    {
        loadInpFile(filename, FileType::Abaqus);
    }
}

void VtkWidget::openGmshFile(const QString &filename)
{
    if (!filename.isEmpty())
    {
        loadInpFile(filename, FileType::Gmsh);
    }
}

void VtkWidget::onCellSelected(size_t cellId, int x, int y)
{
    if (cellId != -1)
    {
        auto cell = grid_->GetCell(cellId);
        auto pointIds = cell->GetPointIds();
        QString tooltipText = QString("Cell ID: %1\n").arg(cellId + 1);
        for (size_t i = 0; i < pointIds->GetNumberOfIds(); i++)
        {
            auto pointId = pointIds->GetId(i);
            auto point = grid_->GetPoint(pointId);
            tooltipText.append(QString("Point ID: %1, Point: %2, %3, %4\n")
                                   .arg(pointId + 1)
                                   .arg(point[0])
                                   .arg(point[1])
                                   .arg(point[2]));
        }

        ui_.tooltip_widget->setText(tooltipText);
        ui_.tooltip_widget->adjustSize();
        ui_.tooltip_widget->setGeometry(QRect(QCursor::pos(), ui_.tooltip_widget->size()));
        ui_.tooltip_widget->show();
        QTimer::singleShot(1000, this, [this]() { ui_.tooltip_widget->close(); });
    }
}

void VtkWidget::onPointSelected(size_t pointId, int x, int y)
{
    Logging::info("Point ID: {}, x: {}, y: {}", pointId, x, y);
    if (pointId != -1)
    {
        auto point = grid_->GetPoint(pointId);
        QString tooltipText = QString("Point ID: %1\n").arg(pointId + 1);
        tooltipText.append(QString("Point: %1, %2, %3\n").arg(point[0]).arg(point[1]).arg(point[2]));
        ui_.tooltip_widget->setText(tooltipText);
        ui_.tooltip_widget->adjustSize();
        ui_.tooltip_widget->setGeometry(QRect(QCursor::pos(), ui_.tooltip_widget->size()));
        ui_.tooltip_widget->show();
        QTimer::singleShot(1000, this, [this]() { ui_.tooltip_widget->close(); });
    }
}
