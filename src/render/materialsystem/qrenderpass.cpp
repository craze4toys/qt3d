/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2016 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qrenderpass.h"
#include "qrenderpass_p.h"
#include "qparameter.h"
#include "qfilterkey.h"
#include "qrenderstate.h"
#include <Qt3DCore/qpropertyupdatedchange.h>
#include <Qt3DCore/qpropertynodeaddedchange.h>
#include <Qt3DCore/qpropertynoderemovedchange.h>
#include <Qt3DCore/private/qnode_p.h>

QT_BEGIN_NAMESPACE

using namespace Qt3DCore;

namespace Qt3DRender {

QRenderPassPrivate::QRenderPassPrivate()
    : QNodePrivate()
    , m_shader(nullptr)
{
}
/*!
 * \qmltype RenderPass
 * \instantiates Qt3DRender::QRenderPass
 * \inqmlmodule Qt3D.Render
 * \brief Encapsulates a Render Pass.
 */

/*!
 * \class Qt3DRender::QRenderPass
 * \inmodule Qt3DRender
 *
 * \inherits Node
 *
 * \brief Encapsulates a Render Pass.
 */

/*!
  \fn Qt3DRender::QRenderPass::QRenderPass(Qt3DCore::QNode *parent)
  Constructs a new QRenderPass with the specified \a parent.
 */
QRenderPass::QRenderPass(QNode *parent)
    : QNode(*new QRenderPassPrivate, parent)
{
}

/*! \internal */
QRenderPass::~QRenderPass()
{
}

/*! \internal */
QRenderPass::QRenderPass(QRenderPassPrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

/*!
  \qmlproperty ShaderProgram Qt3D.Render::RenderPass::shaderProgram
    Specifies the shader program to be used for this render pass
*/

/*!
  \property Qt3DRender::QRenderPass::shaderProgram
    Specifies the shader program to be used for this render pass
 */
/*!
 * Sets the pass' \a shaderProgram.
 */
void QRenderPass::setShaderProgram(QShaderProgram *shaderProgram)
{
    Q_D(QRenderPass);
    if (d->m_shader != shaderProgram) {

        if (d->m_shader != nullptr && d->m_changeArbiter != nullptr) {
            const auto change = QPropertyNodeRemovedChangePtr::create(id(), d->m_shader);
            change->setPropertyName("shaderProgram");
            d->notifyObservers(change);
        }

        if (d->m_shader)
            d->unregisterDestructionHelper(d->m_shader);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, it gets destroyed as well
        if (shaderProgram && !shaderProgram->parent())
            shaderProgram->setParent(this);

        d->m_shader = shaderProgram;

        // Ensures proper bookkeeping
        if (d->m_shader)
            d->registerDestructionHelper(d->m_shader, &QRenderPass::setShaderProgram, d->m_shader);

        emit shaderProgramChanged(shaderProgram);
    }
}

QShaderProgram *QRenderPass::shaderProgram() const
{
    Q_D(const QRenderPass);
    return d->m_shader;
}

/*!
 * Adds \a filterKey from to the Qt3DRender::QRenderPass local filter keys.
 */
void QRenderPass::addFilterKey(QFilterKey *filterKey)
{
    Q_ASSERT(filterKey);
    Q_D(QRenderPass);
    if (!d->m_filterKeyList.contains(filterKey)) {
        d->m_filterKeyList.append(filterKey);

        // Ensures proper bookkeeping
        d->registerDestructionHelper(filterKey, &QRenderPass::removeFilterKey, d->m_filterKeyList);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, it gets destroyed as well
        if (!filterKey->parent())
            filterKey->setParent(this);

        if (d->m_changeArbiter != nullptr) {
            const auto change = QPropertyNodeAddedChangePtr::create(id(), filterKey);
            change->setPropertyName("filterKeys");
            d->notifyObservers(change);
        }
    }
}

/*!
 * Removes \a filterKey from the Qt3DRender::QRenderPass local filter keys.
 */
void QRenderPass::removeFilterKey(QFilterKey *filterKey)
{
    Q_ASSERT(filterKey);
    Q_D(QRenderPass);
    if (d->m_changeArbiter != nullptr) {
        const auto change = QPropertyNodeRemovedChangePtr::create(id(), filterKey);
        change->setPropertyName("filterKeys");
        d->notifyObservers(change);
    }
    d->m_filterKeyList.removeOne(filterKey);
    // Remove bookkeeping connection
    d->unregisterDestructionHelper(filterKey);
}

/*!
 * Returns the list of Qt3DCore::QFilterKey key objects making up the filter keys
 * of the Qt3DRender::QRenderPass.
 */
QVector<QFilterKey *> QRenderPass::filterKeys() const
{
    Q_D(const QRenderPass);
    return d->m_filterKeyList;
}

/*!
 * Adds a Qt3DCore::QRenderState \a state to the rendering pass. That implies that
 * when the pass is executed at render time, the globally set render state will
 * be modifed by the states defined locally by the Qt3DRender::QRenderPass.
 *
 * \note not defining any Qt3DCore::QRenderState in a pass will result in the pass using
 * the globally set render state for a given FrameGraph branch execution path.
 */
void QRenderPass::addRenderState(QRenderState *state)
{
    Q_ASSERT(state);
    Q_D(QRenderPass);
    if (!d->m_renderStates.contains(state)) {
        d->m_renderStates.append(state);

        // Ensures proper bookkeeping
        d->registerDestructionHelper(state, &QRenderPass::removeRenderState, d->m_renderStates);

        if (!state->parent())
            state->setParent(this);

        if (d->m_changeArbiter != nullptr) {
            const auto change = QPropertyNodeAddedChangePtr::create(id(), state);
            change->setPropertyName("renderState");
            d->notifyObservers(change);
        }
    }
}

/*!
 * Removes \a state from the Qt3DRender::QRenderPass local render state.
 */
void QRenderPass::removeRenderState(QRenderState *state)
{
    Q_ASSERT(state);
    Q_D(QRenderPass);
    if (d->m_changeArbiter != nullptr) {
        const auto change = QPropertyNodeRemovedChangePtr::create(id(), state);
        change->setPropertyName("renderState");
        d->notifyObservers(change);
    }
    d->m_renderStates.removeOne(state);
    // Remove bookkeeping connection
    d->unregisterDestructionHelper(state);
}

/*!
 * Returns the list of Qt3DCore::QRenderState state objects making up the render
 * state of the Qt3DRender::QRenderPass.
 */
QVector<QRenderState *> QRenderPass::renderStates() const
{
    Q_D(const QRenderPass);
    return d->m_renderStates;
}

/*!
 * Add \a parameter to the render pass' parameters.
 */
void QRenderPass::addParameter(QParameter *parameter)
{
    Q_ASSERT(parameter);
    Q_D(QRenderPass);
    if (!d->m_parameters.contains(parameter)) {
        d->m_parameters.append(parameter);

        // Ensures proper bookkeeping
        d->registerDestructionHelper(parameter, &QRenderPass::removeParameter, d->m_parameters);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, the child parameters get destroyed as well
        if (!parameter->parent())
            parameter->setParent(this);

        if (d->m_changeArbiter != nullptr) {
            const auto change = QPropertyNodeAddedChangePtr::create(id(), parameter);
            change->setPropertyName("parameter");
            d->notifyObservers(change);
        }
    }
}

/*!
 * Remove \a parameter from the  render pass' parameters.
 */
void QRenderPass::removeParameter(QParameter *parameter)
{
    Q_ASSERT(parameter);
    Q_D(QRenderPass);
    if (d->m_changeArbiter != nullptr) {
        const auto change = QPropertyNodeRemovedChangePtr::create(id(), parameter);
        change->setPropertyName("parameter");
        d->notifyObservers(change);
    }
    d->m_parameters.removeOne(parameter);
    // Remove bookkeeping connection
    d->unregisterDestructionHelper(parameter);
}

/*!
 * Returns a vector of the render pass' current parameters
 */
QVector<QParameter *> QRenderPass::parameters() const
{
    Q_D(const QRenderPass);
    return d->m_parameters;
}

Qt3DCore::QNodeCreatedChangeBasePtr QRenderPass::createNodeCreationChange() const
{
    auto creationChange = Qt3DCore::QNodeCreatedChangePtr<QRenderPassData>::create(this);
    auto &data = creationChange->data;
    Q_D(const QRenderPass);
    data.filterKeyIds = qIdsForNodes(d->m_filterKeyList);
    data.parameterIds = qIdsForNodes(d->m_parameters);
    data.renderStateIds = qIdsForNodes(d->m_renderStates);
    data.shaderId = qIdForNode(d->m_shader);
    return creationChange;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
