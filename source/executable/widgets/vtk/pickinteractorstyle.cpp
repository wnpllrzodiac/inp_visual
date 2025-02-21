#include "pickinteractorstyle.h"
#include <QCursor>
#include <QToolTip>
#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkObjectFactory.h>
#include <vtkPointPicker.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkUnstructuredGrid.h>

#include "utils/logging.h"
vtkStandardNewMacro(CellPickInteractorStyle);
vtkStandardNewMacro(PointPickInteractorStyle);

void CellPickInteractorStyle::OnLeftButtonDown()
{
    vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.0005);

    int *clickPos = this->GetInteractor()->GetEventPosition();
    vtkRenderer *renderer = this->GetDefaultRenderer();
    if (renderer)
    {
        picker->Pick(clickPos[0], clickPos[1], 0, renderer);

        vtkIdType cellId = picker->GetCellId();
        if (cellId != -1)
        {
            vtkSmartPointer<vtkUnstructuredGrid> grid = vtkUnstructuredGrid::SafeDownCast(picker->GetDataSet());
            if (grid)
            {
                vtkCell *cell = grid->GetCell(cellId);
                if (cell)
                {
                    if (emitter_)
                    {
                        emitter_->cellSelected(static_cast<size_t>(cellId), clickPos[0], clickPos[1]);
                    }
                    if (highlight_actor_)
                    {
                        renderer->RemoveActor(highlight_actor_);
                        highlight_actor_ = nullptr;
                    }
                    // 创建一个新的UnstructuredGrid，只包含选中的单元格
                    vtkSmartPointer<vtkUnstructuredGrid> singleCellGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
                    singleCellGrid->Allocate(1);
                    singleCellGrid->InsertNextCell(cell->GetCellType(), cell->GetPointIds());

                    // 设置点数据
                    singleCellGrid->SetPoints(grid->GetPoints());

                    // 高亮单元格
                    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
                    mapper->SetInputData(singleCellGrid);

                    highlight_actor_ = vtkSmartPointer<vtkActor>::New();
                    highlight_actor_->SetMapper(mapper);
                    highlight_actor_->GetProperty()->SetColor(0, 1, 0); // 设置高亮颜色为绿色
                    highlight_actor_->GetProperty()->SetEdgeVisibility(true);
                    highlight_actor_->GetProperty()->SetLineWidth(2);

                    renderer->AddActor(highlight_actor_);
                    renderer->GetRenderWindow()->Render();
                }
            }
        }
    }
    else
    {
        Logging::error("Error: No renderer available for picking.");
    }

    vtkInteractorStyleRubberBand2D::OnLeftButtonDown();
}

void CellPickInteractorStyle::setEmitter(SignalEmitter *emitter) { emitter_ = emitter; }

void PointPickInteractorStyle::OnLeftButtonDown()
{
    const double epsilon = 0.005;
    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    picker->SetTolerance(epsilon);

    int *clickPos = this->GetInteractor()->GetEventPosition();
    vtkRenderer *renderer = this->GetDefaultRenderer();
    if (renderer)
    {
        picker->Pick(clickPos[0], clickPos[1], 0, renderer);

        vtkIdType pointId = picker->GetPointId();
        if (pointId != -1)
        {
            vtkSmartPointer<vtkUnstructuredGrid> grid = vtkUnstructuredGrid::SafeDownCast(picker->GetDataSet());
            if (grid == nullptr)
            {
                Logging::error("Error: No grid available for picking.");
                return;
            }
            auto point = grid->GetPoint(pointId);

            bool isSamePoint = highlight_actor_ && std::abs(highlight_actor_->GetPosition()[0] - point[0]) < epsilon &&
                               std::abs(highlight_actor_->GetPosition()[1] - point[1]) < epsilon &&
                               std::abs(highlight_actor_->GetPosition()[2] - point[2]) < epsilon;

            if (isSamePoint)
            {
                return;
            }

            if (highlight_actor_)
            {
                renderer->RemoveActor(highlight_actor_);
                highlight_actor_ = nullptr;
            }

            // 高亮显示选中的点
            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetCenter(point);
            sphereSource->SetRadius(0.1);
            sphereSource->Update();

            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphereSource->GetOutputPort());

            highlight_actor_ = vtkSmartPointer<vtkActor>::New();
            highlight_actor_->SetMapper(mapper);
            highlight_actor_->GetProperty()->SetColor(1, 0, 0); // 设置高亮颜色为红色

            renderer->AddActor(highlight_actor_);
            renderer->GetRenderWindow()->Render();
            emitter_->pointSelected(static_cast<size_t>(pointId), clickPos[0], clickPos[1]);
        }
    }
    else
    {
        Logging::error("Error: No renderer available for picking.");
    }

    vtkInteractorStyleRubberBand2D::OnLeftButtonDown();
}

void PointPickInteractorStyle::setEmitter(SignalEmitter *emitter) { emitter_ = emitter; }
