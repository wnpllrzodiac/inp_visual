#include "vtkwidget.h"
#include "inpreader.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkProperty.h>

VtkWidget::VtkWidget(QWidget* parent)
    : QWidget(parent)
{
    // 初始化渲染窗口
    initUi();

    // 默认加载测试文件
    loadInpFile("path/to/your/default.inp");
}

void VtkWidget::initUi()
{
    // 创建VTK OpenGL部件
    render_window_ = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderer_ = vtkSmartPointer<vtkRenderer>::New();
    render_window_->AddRenderer(renderer_);

    vtk_widget_ = new QVTKOpenGLNativeWidget(this);
    vtk_widget_->setRenderWindow(render_window_);

    renderer_->SetBackground(0.2, 0.3, 0.4);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(vtk_widget_);
    layout->setContentsMargins(0, 0, 0, 0);
}

vtkRenderer* VtkWidget::renderer() const
{
    return renderer_.Get();
}

void VtkWidget::resetCamera()
{
    renderer_->ResetCamera();
}

void VtkWidget::loadInpFile(const QString& filename)
{
    // 清空当前场景
    renderer_->RemoveAllViewProps();

    // 创建解析器并读取文件
    InpReader reader;
    auto grid = reader.parse(filename);

    if (grid && grid->GetNumberOfCells() > 0) {
        // 创建Mapper
        auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputData(grid);

        // 创建Actor
        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->EdgeVisibilityOn();
        actor->GetProperty()->SetEdgeColor(0.2, 0.2, 0.2);

        // 添加到渲染器
        renderer_->AddActor(actor);
        renderer_->ResetCamera();
        render_window_->Render();
    } else {
        qWarning() << "Failed to load or parse INP file:" << filename;
    }
}

void VtkWidget::openInpFile(const QString& filename)
{

    if (!filename.isEmpty()) {
        loadInpFile(filename);
    }
}