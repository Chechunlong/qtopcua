/****************************************************************************
**
** Copyright (C) 2017 basysKom GmbH, opensource@basyskom.com
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOpcUa module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QOPCUABACKEND_P_H
#define QOPCUABACKEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtOpcUa/qopcuaclient.h>
#include <private/qopcuanodeimpl_p.h>

#include <QtCore/qobject.h>

#include <functional>

QT_BEGIN_NAMESPACE

class QOpcUaMonitoringParameters;

class Q_OPCUA_EXPORT QOpcUaBackend : public QObject
{
    Q_OBJECT

public:
    explicit QOpcUaBackend();
    virtual ~QOpcUaBackend();

    static Q_DECL_CONSTEXPR size_t nodeAttributeEnumBits()
    {
        return sizeof(std::underlying_type<QOpcUa::NodeAttribute>::type) * CHAR_BIT;
    }

    QOpcUa::Types attributeIdToTypeId(QOpcUa::NodeAttribute attr);

    double revisePublishingInterval(double requestedValue, double minimumValue);

Q_SIGNALS:
    void stateAndOrErrorChanged(QOpcUaClient::ClientState state,
                                QOpcUaClient::ClientError error);
    void attributesRead(uintptr_t handle, QVector<QOpcUaReadResult> attributes, QOpcUa::UaStatusCode serviceResult);
    void attributeWritten(uintptr_t hande, QOpcUa::NodeAttribute attribute, QVariant value, QOpcUa::UaStatusCode statusCode);
    void methodCallFinished(uintptr_t handle, QString methodNodeId, QVariant result, QOpcUa::UaStatusCode statusCode);

    void attributeUpdated(uintptr_t handle, QOpcUaReadResult res);
    void monitoringEnableDisable(uintptr_t handle, QOpcUa::NodeAttribute attr, bool subscribe, QOpcUaMonitoringParameters status);
    void monitoringStatusChanged(uintptr_t handle, QOpcUa::NodeAttribute attr, QOpcUaMonitoringParameters::Parameters items,
                           QOpcUaMonitoringParameters param);
    void browseFinished(uintptr_t handle, QVector<QOpcUaReferenceDescription> children, QOpcUa::UaStatusCode statusCode);

private:
    Q_DISABLE_COPY(QOpcUaBackend)
};

static inline void qt_forEachAttribute(QOpcUa::NodeAttributes attributes, const std::function<void(QOpcUa::NodeAttribute attribute)> &f)
{
    for (uint i = 0; i < QOpcUaBackend::nodeAttributeEnumBits(); ++i) {
        if (!(attributes & (1 << i)))
            continue;
        QOpcUa::NodeAttribute currentAttribute = static_cast<QOpcUa::NodeAttribute>(1 << i);
        f(currentAttribute);
    }
}

QT_END_NAMESPACE

#endif // QOPCUABACKEND_P_H
