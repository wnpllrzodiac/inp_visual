#include "configwidget.h"
#include "config/boundaryconditionswidget.h"
#include "config/controlparameterswidget.h"
#include "config/dfnwidget.h"
#include "config/elementpropertieswidget.h"
#include "config/jointpropertieswidget.h"
#include "config/unitwidget.h"


ConfigWidget::ConfigWidget(QWidget *parent)
: QTabWidget(parent)
{
    initUi();
    setupConnections();
}

void ConfigWidget::initUi()
{
    // 单位
    addTab(new UnitWidget(), tr("unit"));

    // 控制参数
    addTab(new ControlParametersWidget(), tr("control_parameters"));

    // 单元属性
    addTab(new ElementPropertiesWidget(), tr("element_properties"));

    // 节理属性
    addTab(new JointPropertiesWidget(), tr("joint_properties"));

    // DFN
    addTab(new DFNWidget(), tr("DFN"));

    // 边界条件
    addTab(new BoundaryConditionsWidget(), tr("boundary_conditions"));
}

void ConfigWidget::setupConnections() { }
