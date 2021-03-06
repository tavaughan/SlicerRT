/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// SlicerRt includes
#include "vtkSlicerRtCommon.h"

// Isodose includes
#include "qSlicerSubjectHierarchyIsodosePlugin.h"
#include "vtkMRMLIsodoseNode.h"
#include "vtkSlicerIsodoseModuleLogic.h"

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyConstants.h"
#include "vtkMRMLSubjectHierarchyNode.h"

// SubjectHierarchy Plugins includes
#include "qSlicerSubjectHierarchyPluginHandler.h"
#include "qSlicerSubjectHierarchyDefaultPlugin.h"

// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLModelNode.h>

// MRML Widgets includes
#include <qMRMLNodeComboBox.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkCollection.h>

// Qt includes
#include <QDebug>
#include <QIcon>
#include <QStandardItem>
#include <QAction>

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup SlicerRt_QtModules_RtHierarchy
class qSlicerSubjectHierarchyIsodosePluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qSlicerSubjectHierarchyIsodosePlugin);
protected:
  qSlicerSubjectHierarchyIsodosePlugin* const q_ptr;
public:
  qSlicerSubjectHierarchyIsodosePluginPrivate(qSlicerSubjectHierarchyIsodosePlugin& object);
  ~qSlicerSubjectHierarchyIsodosePluginPrivate();
public:
  QIcon IsodoseLinesIcon;
};

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyIsodosePluginPrivate methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyIsodosePluginPrivate::qSlicerSubjectHierarchyIsodosePluginPrivate(qSlicerSubjectHierarchyIsodosePlugin& object)
 : q_ptr(&object)
{
  this->IsodoseLinesIcon = QIcon(":Icons/IsodoseLines.png");
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyIsodosePluginPrivate::~qSlicerSubjectHierarchyIsodosePluginPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerSubjectHierarchyIsodosePlugin methods

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyIsodosePlugin::qSlicerSubjectHierarchyIsodosePlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qSlicerSubjectHierarchyIsodosePluginPrivate(*this) )
{
  this->m_Name = QString("Isodose");
}

//-----------------------------------------------------------------------------
qSlicerSubjectHierarchyIsodosePlugin::~qSlicerSubjectHierarchyIsodosePlugin()
{
}

//---------------------------------------------------------------------------
double qSlicerSubjectHierarchyIsodosePlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (!itemID)
  {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return 0.0;
  }
  vtkMRMLSubjectHierarchyNode* shNode = qSlicerSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
  {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
  }

  // Isodose lines
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && vtkSlicerRtCommon::IsIsodoseModelNode(associatedNode))
  {
    return 1.0;
  }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qSlicerSubjectHierarchyIsodosePlugin::roleForPlugin()const
{
  return "Isodose surface";
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyIsodosePlugin::icon(vtkIdType itemID)
{
  Q_D(qSlicerSubjectHierarchyIsodosePlugin);

  if (!itemID)
  {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
  }

  if (this->canOwnSubjectHierarchyItem(itemID))
  {
    return d->IsodoseLinesIcon;
  }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qSlicerSubjectHierarchyIsodosePlugin::visibilityIcon(int visible)
{
  // Have the default plugin (which is not registered) take care of this
  return qSlicerSubjectHierarchyPluginHandler::instance()->defaultPlugin()->visibilityIcon(visible);
}

//---------------------------------------------------------------------------
void qSlicerSubjectHierarchyIsodosePlugin::editProperties(vtkIdType itemID)
{
  Q_UNUSED(itemID);

  // Switch to isodose module with parameter set node already selected
  qSlicerAbstractModuleWidget* moduleWidget = qSlicerSubjectHierarchyAbstractPlugin::switchToModule("Isodose");
  if (moduleWidget)
  {
    // Get node selector combobox
    qMRMLNodeComboBox* nodeSelector = moduleWidget->findChild<qMRMLNodeComboBox*>("MRMLNodeComboBox_ParameterSet");

    //TODO: Get parameter set node for isodose model and model hierarchy node
    // and set corresponding dose volume too when changes in #580 are implemented
    vtkMRMLIsodoseNode* isodoseParameterSetNode = NULL; //TODO:

    // Choose current data node
    if (nodeSelector && isodoseParameterSetNode)
    {
      nodeSelector->setCurrentNode(isodoseParameterSetNode);
    }
  }
}
