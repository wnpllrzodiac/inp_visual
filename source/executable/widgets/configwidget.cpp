#include "configwidget.h"
#include "config/boundaryconditionswidget.h"
#include "config/controlparameterswidget.h"
#include "config/dfnwidget.h"
#include "config/elementpropertieswidget.h"
#include "config/jointpropertieswidget.h"
#include "config/unitwidget.h"
#include "projectmodel.h"

ConfigWidget::ConfigWidget(QWidget *parent, ProjectModel *project_model)
: QTabWidget(parent)
, project_model_(project_model)
{
    initUi();
    setupConnections();
}

void ConfigWidget::initUi()
{
    // 单位
    addTab(new UnitWidget(this, project_model_), tr("unit"));

    // 控制参数
    addTab(new ControlParametersWidget(this, project_model_), tr("control_parameters"));

    // 单元属性
    addTab(new ElementPropertiesWidget(this, project_model_), tr("element_properties"));

    // 边界条件
    addTab(new BoundaryConditionsWidget(this, project_model_), tr("boundary_conditions"));
    // 节理属性
    addTab(new JointPropertiesWidget(this, project_model_), tr("joint_properties"));

    // DFN
    addTab(new DFNWidget(this, project_model_), tr("DFN"));
}

void ConfigWidget::setupConnections() { }
