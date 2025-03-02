#include "projectmodel.h"

ProjectModel::ProjectModel(QObject *parent)
: QObject(parent)
{
}

ProjectModel::~ProjectModel() = default;

void ProjectModel::setLengthUnit(LengthUnit unit)
{
    if (unit_settings_.length_unit != unit)
    {
        unit_settings_.length_unit = unit;
        emit lengthUnitChanged(unit);
        emit unitSettingsChanged();
    }
}

void ProjectModel::setMassUnit(MassUnit unit)
{
    if (unit_settings_.mass_unit != unit)
    {
        unit_settings_.mass_unit = unit;
        emit massUnitChanged(unit);
        emit unitSettingsChanged();
    }
}

void ProjectModel::setTimeUnit(TimeUnit unit)
{
    if (unit_settings_.time_unit != unit)
    {
        unit_settings_.time_unit = unit;
        emit timeUnitChanged(unit);
        emit unitSettingsChanged();
    }
}

void ProjectModel::setTimeSteps(int steps)
{
    if (control_settings_.time_steps != steps)
    {
        control_settings_.time_steps = steps;
        emit timeStepsChanged(steps);
        emit controlSettingsChanged();
    }
}

void ProjectModel::setOutputFrequency(int freq)
{
    if (control_settings_.output_frequency != freq)
    {
        control_settings_.output_frequency = freq;
        emit outputFrequencyChanged(freq);
        emit controlSettingsChanged();
    }
}

void ProjectModel::setMinElementSize(int size)
{
    if (control_settings_.min_element_size != size)
    {
        control_settings_.min_element_size = size;
        emit minElementSizeChanged(size);
        emit controlSettingsChanged();
    }
}

void ProjectModel::setTimeStep(double step)
{
    if (control_settings_.time_step != step)
    {
        control_settings_.time_step = step;
        emit timeStepChanged(step);
        emit controlSettingsChanged();
    }
}

void ProjectModel::setGravity(double gravity)
{
    if (control_settings_.gravity != gravity)
    {
        control_settings_.gravity = gravity;
        emit gravityChanged(gravity);
        emit controlSettingsChanged();
    }
}

void ProjectModel::addElementProperty(const ElementProperty &prop)
{
    element_properties_.append(prop);
    emit elementPropertyAdded(prop);
}

void ProjectModel::updateElementProperty(QString element_id, const ElementProperty &prop)
{
    auto it = std::find_if(element_properties_.begin(),
                           element_properties_.end(),
                           [element_id](const ElementProperty &p) { return p.id == element_id; });
    if (it != element_properties_.end())
    {
        *it = prop;
        emit elementPropertyUpdated(element_id, prop);
    }
}

void ProjectModel::removeElementProperty(QString element_id)
{
    for (auto it = element_properties_.begin(); it != element_properties_.end(); ++it)
    {
        if (it->id == element_id)
        {
            element_properties_.erase(it);
            emit elementPropertyRemoved(element_id);
            break;
        }
    }
}

void ProjectModel::addBoundaryCondition(const BoundaryCondition &condition)
{
    boundary_conditions_.append(condition);
    emit boundaryConditionAdded(condition);
}

void ProjectModel::updateBoundaryCondition(QString boundary_id, const BoundaryCondition &condition)
{
    auto it = std::find_if(boundary_conditions_.begin(),
                           boundary_conditions_.end(),
                           [boundary_id](const BoundaryCondition &b) { return b.id == boundary_id; });
    if (it != boundary_conditions_.end())
    {
        *it = condition;
        emit boundaryConditionUpdated(boundary_id, condition);
    }
}

void ProjectModel::removeBoundaryCondition(QString boundary_id)
{
    for (auto it = boundary_conditions_.begin(); it != boundary_conditions_.end(); ++it)
    {
        if (it->id == boundary_id)
        {
            boundary_conditions_.erase(it);
            emit boundaryConditionRemoved(boundary_id);
            break;
        }
    }
}

void ProjectModel::setNodeSets(const QMap<QString, std::vector<int>> &node_sets)
{
    node_sets_ = node_sets;
    emit nodeSetsChanged();
}

BoundaryCondition ProjectModel::getBoundaryCondition(int node_id) const
{
    QString node_set;
    for (auto it = node_sets_.begin(); it != node_sets_.end(); ++it)
    {
        if (std::find(it.value().begin(), it.value().end(), node_id + 1) != it.value().end())
        {
            node_set = it.key();
            break;
        }
    }

    for (const auto &bc : boundary_conditions_)
    {
        if (bc.id == node_set)
        {
            return bc;
        }
    }
    return {};
}

void ProjectModel::setElementSets(const QMap<QString, std::vector<int>> &element_sets)
{
    element_sets_ = element_sets;
    emit elementSetsChanged();
}

ElementProperty ProjectModel::getCellProperty(int cell_id) const
{
    QString element_set;
    for (auto it = element_sets_.begin(); it != element_sets_.end(); ++it)
    {
        if (std::find(it.value().begin(), it.value().end(), cell_id + 1) != it.value().end())
        {
            element_set = it.key();
            break;
        }
    }

    for (const auto &ep : element_properties_)
    {
        if (ep.id == element_set)
        {
            return ep;
        }
    }
    return {};
}