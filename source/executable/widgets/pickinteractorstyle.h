#ifndef PICKINTERACTORSTYLE_H
#define PICKINTERACTORSTYLE_H

#include "utils/vtksignal.h"
#include <vtkBalloonWidget.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkInteractorStyleRubberBand2D.h>
#include <vtkSmartPointer.h>

class CellPickInteractorStyle : public vtkInteractorStyleRubberBand2D
{
public:
    static CellPickInteractorStyle *New();
    vtkTypeMacro(CellPickInteractorStyle, vtkInteractorStyleRubberBand2D);

    virtual void OnLeftButtonDown() override;

    vtkSmartPointer<vtkEventQtSlotConnect> Connections;
    void setEmitter(SignalEmitter *emitter);

private:
    SignalEmitter *emitter_ = nullptr;
};

class PointPickInteractorStyle : public vtkInteractorStyleRubberBand2D
{
public:
    static PointPickInteractorStyle *New();
    vtkTypeMacro(PointPickInteractorStyle, vtkInteractorStyleRubberBand2D);

    virtual void OnLeftButtonDown() override;
    void setEmitter(SignalEmitter *emitter);

private:
    SignalEmitter *emitter_ = nullptr;
    vtkSmartPointer<vtkActor> highlightedActor_;
};

#endif // PICKINTERACTORSTYLE_H