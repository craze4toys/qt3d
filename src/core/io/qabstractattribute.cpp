/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qabstractattribute.h"
#include "qabstractattribute_p.h"
#include <qabstractbuffer.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QAbstractAttributePrivate::QAbstractAttributePrivate(QAbstractAttribute *qq)
    : q_ptr(qq)
{
}

QAbstractAttribute::QAbstractAttribute(QAbstractAttributePrivate &dd, QAbstractBufferPtr buf, int type, uint count, uint offset, uint stride)
    : d_ptr(&dd)
{
    Q_D(QAbstractAttribute);
    d->m_buffer = buf;
    d->m_count = count;
    d->m_divisor = 0;
    d->m_offset = offset;
    d->m_type = type;
    d->m_stride = stride;
}

QAbstractAttribute::QAbstractAttribute(QAbstractBufferPtr buf, int type, uint count, uint offset, uint stride)
    : d_ptr(new QAbstractAttributePrivate(this))
{
    Q_D(QAbstractAttribute);
    d->m_buffer = buf;
    d->m_count = count;
    d->m_divisor = 0;
    d->m_offset = offset;
    d->m_type = type;
    d->m_stride = stride;
}

QAbstractAttribute::~QAbstractAttribute()
{
}

int QAbstractAttribute::type() const
{
    Q_D(const QAbstractAttribute);
    return d->m_type;
}

uint QAbstractAttribute::count() const
{
    Q_D(const QAbstractAttribute);
    return d->m_count;
}

uint QAbstractAttribute::byteStride() const
{
    Q_D(const QAbstractAttribute);
    return d->m_stride;
}

uint QAbstractAttribute::byteOffset()
{
    Q_D(const QAbstractAttribute);
    return d->m_offset;
}

uint QAbstractAttribute::divisor() const
{
    Q_D(const QAbstractAttribute);
    return d->m_divisor;
}

QAbstractBufferPtr QAbstractAttribute::buffer() const
{
    Q_D(const QAbstractAttribute);
    return d->m_buffer;
}

} // Qt3D

QT_END_NAMESPACE
