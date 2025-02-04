#include "vtkwidget.h"
#include "inpreader.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <vtkActor.h>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
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

VtkWidget::VtkWidget(QWidget* parent)
    : QWidget(parent)
{
    // 初始化渲染窗口
    initUi();
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

void VtkWidget::loadInpFile(const QString& filename, const FileType& type)
{
    // 清空当前场景
    renderer_->RemoveAllViewProps();

    // 创建解析器并读取文件
    InpReader reader;
    vtkSmartPointer<vtkUnstructuredGrid> grid;
    if (type == FileType::Abaqus) {
        grid = reader.parseAbaqus(filename);
    } else if (type == FileType::Gmsh) {
        grid = reader.parseGmsh(filename);
    }

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

void VtkWidget::openAbaqusFile(const QString& filename)
{
    if (!filename.isEmpty()) {
        loadInpFile(filename, FileType::Abaqus);
    }
}

void VtkWidget::openGmshFile(const QString& filename)
{
    if (!filename.isEmpty()) {
        loadInpFile(filename, FileType::Gmsh);
    }
}

void VtkWidget::openVtkFile(const QString& filename)
{
    if (!filename.isEmpty()) {
        // 清空当前场景
        renderer_->RemoveAllViewProps();

        // 读取VTK文件
        auto reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        reader->Update();

        // 创建Mapper（读取三角面）
        auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputConnection(reader->GetOutputPort());

        // 创建Actor用于显示三角面
        auto actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->EdgeVisibilityOn();
        actor->GetProperty()->SetEdgeColor(0.2, 0.2, 0.2);
        // 可选：设置为表面渲染，采用平面内插以增强边缘感
        actor->GetProperty()->SetRepresentationToSurface();
        actor->GetProperty()->SetInterpolationToFlat();

        // 添加三角面Actor到渲染器
        renderer_->AddActor(actor);

        // 增加显示数据点的Actor
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkUnstructuredGrid::SafeDownCast(reader->GetOutput());
        if (grid && grid->GetNumberOfPoints() > 0) {
            auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
            vertexFilter->SetInputData(grid);
            vertexFilter->Update();

            auto pointsMapper = vtkSmartPointer<vtkDataSetMapper>::New();
            pointsMapper->SetInputConnection(vertexFilter->GetOutputPort());

            auto pointsActor = vtkSmartPointer<vtkActor>::New();
            pointsActor->SetMapper(pointsMapper);
            // 设置数据点颜色为红色，并设定合适的点大小
            pointsActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
            pointsActor->GetProperty()->SetPointSize(5);

            renderer_->AddActor(pointsActor);
        }

        renderer_->ResetCamera();
        render_window_->Render();
    }
}

void VtkWidget::openVtuFile(const QString& filename)
{
    if (!filename.isEmpty()) {
        // 清空当前场景
        renderer_->RemoveAllViewProps();

        // 读取VTU文件
        auto reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
        reader->SetFileName(filename.toStdString().c_str());
        reader->Update();
        auto unstructuredGrid = reader->GetOutput();
        saveUnstructuredGrid(filename + "_ss.vtu", unstructuredGrid);

        // 定义预设颜色（红、绿、蓝、黄、品红、青）
        std::vector<std::array<double, 3>> colors = {
            { 1.0, 0.0, 0.0 }, // red
            { 0.0, 1.0, 0.0 }, // green
            { 0.0, 0.0, 1.0 }, // blue
            { 1.0, 1.0, 0.0 }, // yellow
            { 1.0, 0.0, 1.0 }, // magenta
            { 0.0, 1.0, 1.0 } // cyan
        };

        // ----- 根据单元组Elset采用不同颜色显示结构 -----
        auto celldata = unstructuredGrid->GetCellData();
        for (int i = 0; i < celldata->GetNumberOfArrays(); ++i) {
            vtkSmartPointer<vtkDataArray> dataArray = celldata->GetArray(i);
            if (dataArray) {

                std::cout << "  - Name: " << dataArray->GetName() << ", Number of Components: " << dataArray->GetNumberOfComponents()
                          << ", Number of Tuples: " << dataArray->GetNumberOfTuples() << std::endl;
            }
        }

        auto elsetArray = celldata->GetArray("elset");
        if (!elsetArray)
            elsetArray = celldata->GetArray("ELSET");

        // vtkDataArray* elsetArray = unstructuredGrid->GetCellData()->GetArray("elset");
        if (!elsetArray)
            elsetArray = unstructuredGrid->GetCellData()->GetArray("ELSET");

        if (elsetArray) {
            std::set<int> uniqueSets;
            vtkIdType numCells = unstructuredGrid->GetNumberOfCells();
            for (vtkIdType i = 0; i < numCells; i++) {
                double value = elsetArray->GetComponent(i, 0);
                uniqueSets.insert(static_cast<int>(value));
            }
            int colorIndex = 0;
            for (int setId : uniqueSets) {
                auto threshold = vtkSmartPointer<vtkThreshold>::New();
                threshold->SetInputData(unstructuredGrid);
                threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, elsetArray->GetName());
                threshold->SetLowerThreshold(setId);
                threshold->SetUpperThreshold(setId);
                threshold->Update();

                auto geometryFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
                geometryFilter->SetInputConnection(threshold->GetOutputPort());
                geometryFilter->Update();

                auto cellMapper = vtkSmartPointer<vtkDataSetMapper>::New();
                cellMapper->SetInputConnection(geometryFilter->GetOutputPort());

                auto cellActor = vtkSmartPointer<vtkActor>::New();
                cellActor->SetMapper(cellMapper);
                cellActor->GetProperty()->EdgeVisibilityOn();
                cellActor->GetProperty()->SetEdgeColor(0.2, 0.2, 0.2);
                cellActor->GetProperty()->SetRepresentationToSurface();
                cellActor->GetProperty()->SetInterpolationToFlat();
                auto color = colors[colorIndex % colors.size()];
                cellActor->GetProperty()->SetColor(color[0], color[1], color[2]);

                renderer_->AddActor(cellActor);
                colorIndex++;
            }
        } else {
            // 没有Elset信息，使用单一颜色显示所有单元
            auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
            mapper->SetInputData(unstructuredGrid);

            auto actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            actor->GetProperty()->EdgeVisibilityOn();
            actor->GetProperty()->SetEdgeColor(0.2, 0.2, 0.2);
            actor->GetProperty()->SetRepresentationToSurface();
            actor->GetProperty()->SetInterpolationToFlat();
            renderer_->AddActor(actor);
        }

        // ----- 根据节点集Nset采用不同颜色显示节点 -----
        vtkDataArray* nsetArray = unstructuredGrid->GetPointData()->GetArray("nset");
        if (!nsetArray)
            nsetArray = unstructuredGrid->GetPointData()->GetArray("NSET");
        if (nsetArray) {
            std::set<int> uniquePointSets;
            vtkIdType numPoints = unstructuredGrid->GetNumberOfPoints();
            for (vtkIdType i = 0; i < numPoints; i++) {
                double value = nsetArray->GetComponent(i, 0);
                uniquePointSets.insert(static_cast<int>(value));
            }
            int colorIndex = 0;
            for (int setId : uniquePointSets) {
                auto thresholdPoints = vtkSmartPointer<vtkThresholdPoints>::New();
                thresholdPoints->SetInputData(unstructuredGrid);
                thresholdPoints->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, nsetArray->GetName());
                thresholdPoints->SetLowerThreshold(setId);
                thresholdPoints->SetUpperThreshold(setId);
                thresholdPoints->Update();

                auto pointsMapper = vtkSmartPointer<vtkDataSetMapper>::New();
                pointsMapper->SetInputConnection(thresholdPoints->GetOutputPort());

                auto pointsActor = vtkSmartPointer<vtkActor>::New();
                pointsActor->SetMapper(pointsMapper);
                auto color = colors[colorIndex % colors.size()];
                pointsActor->GetProperty()->SetColor(color[0], color[1], color[2]);
                pointsActor->GetProperty()->SetPointSize(5);

                renderer_->AddActor(pointsActor);
                colorIndex++;
            }
        } else {
            // 如果没有Nset信息，则显示所有节点为红色
            if (unstructuredGrid && unstructuredGrid->GetNumberOfPoints() > 0) {
                auto vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
                vertexFilter->SetInputData(unstructuredGrid);
                vertexFilter->Update();

                auto pointsMapper = vtkSmartPointer<vtkDataSetMapper>::New();
                pointsMapper->SetInputConnection(vertexFilter->GetOutputPort());

                auto pointsActor = vtkSmartPointer<vtkActor>::New();
                pointsActor->SetMapper(pointsMapper);
                pointsActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
                pointsActor->GetProperty()->SetPointSize(5);

                renderer_->AddActor(pointsActor);
            }
        }

        renderer_->ResetCamera();
        render_window_->Render();
    }
}

void VtkWidget::saveUnstructuredGrid(const QString& file_path, vtkUnstructuredGrid* grid)
{
    if (!grid) {
        qWarning() << "无效的 unstructuredGrid 数据，保存失败！";
        return;
    }
    // 创建VTU写入器
    auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(file_path.toStdString().c_str());
    writer->SetInputData(grid);
    writer->Write();
}
