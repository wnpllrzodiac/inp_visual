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
                    emitter_->cellSelected(static_cast<size_t>(cellId), clickPos[0], clickPos[1]);

                    // 创建一个新的UnstructuredGrid，只包含选中的单元格
                    vtkSmartPointer<vtkUnstructuredGrid> singleCellGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
                    singleCellGrid->Allocate(1);
                    singleCellGrid->InsertNextCell(cell->GetCellType(), cell->GetPointIds());

                    // 设置点数据
                    singleCellGrid->SetPoints(grid->GetPoints());

                    // 高亮单元格
                    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
                    mapper->SetInputData(singleCellGrid);

                    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
                    actor->SetMapper(mapper);
                    actor->GetProperty()->SetColor(0, 1, 0); // 设置高亮颜色为绿色
                    actor->GetProperty()->SetEdgeVisibility(true);
                    actor->GetProperty()->SetLineWidth(2);

                    renderer->AddActor(actor);
                    renderer->GetRenderWindow()->Render();
                }
            }
        }
    }
    else
    {
        std::cerr << "Error: No renderer available for picking." << std::endl;
    }

    vtkInteractorStyleRubberBand2D::OnLeftButtonDown();
}

void CellPickInteractorStyle::setEmitter(SignalEmitter *emitter) { emitter_ = emitter; }

void PointPickInteractorStyle::OnLeftButtonDown()
{
    vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
    picker->SetTolerance(0.01);

    int *clickPos = this->GetInteractor()->GetEventPosition();
    vtkRenderer *renderer = this->GetDefaultRenderer();
    if (renderer)
    {
        picker->Pick(clickPos[0], clickPos[1], 0, renderer);

        vtkIdType pointId = picker->GetPointId();
        if (pointId != -1)
        {
            vtkSmartPointer<vtkUnstructuredGrid> grid = vtkUnstructuredGrid::SafeDownCast(picker->GetDataSet());
            auto point = grid->GetPoint(pointId);

            // 检查点击的点是否与高亮的Actor位置相同
            if (this->highlightedActor_)
            {
                double highlightedPoint[3];
                this->highlightedActor_->GetPosition(highlightedPoint);
                if (highlightedPoint[0] == point[0] && highlightedPoint[1] == point[1] &&
                    highlightedPoint[2] == point[2])
                {
                    // 如果点击的是高亮的Actor，直接返回
                    return;
                }
                // 移除之前高亮的Actor
                renderer->RemoveActor(this->highlightedActor_);
            }

            // 高亮显示选中的点
            vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetCenter(point);
            sphereSource->SetRadius(0.1);
            sphereSource->Update();

            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(sphereSource->GetOutputPort());

            this->highlightedActor_ = vtkSmartPointer<vtkActor>::New();
            this->highlightedActor_->SetMapper(mapper);
            this->highlightedActor_->GetProperty()->SetColor(1, 0, 0); // 设置高亮颜色为红色

            renderer->AddActor(this->highlightedActor_);
            renderer->GetRenderWindow()->Render();
            emitter_->pointSelected(static_cast<size_t>(pointId), clickPos[0], clickPos[1]);
        }
    }
    else
    {
        std::cerr << "Error: No renderer available for picking." << std::endl;
    }

    vtkInteractorStyleRubberBand2D::OnLeftButtonDown();
}

void PointPickInteractorStyle::setEmitter(SignalEmitter *emitter) { emitter_ = emitter; }
