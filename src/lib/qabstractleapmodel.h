#ifndef QABSTRACTLEAPMODEL_H
#define QABSTRACTLEAPMODEL_H

#include <QtCore/QAbstractListModel>
#include "qtleap_global.h"

class QTLEAP_EXPORT QAbstractLeapModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
public:
    explicit QAbstractLeapModel(QObject *parent = 0);
    
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QVariantMap get(int row) const;
    int count() const;
    bool isConnected() const;
    bool isEnabled() const;

public slots:
    void setEnabled(bool enabled);

signals:
    void countChanged(int count);
    void connectedChanged(bool connected);
    void enabledChanged(bool enabled);

protected:
    void beginUpdate();
    void upsert(int id, const QVariantList &data);
    void endUpdate();
    virtual void timeout(const Leap::Controller &controller) = 0;
    Leap::Controller &controller() const;

private:
    class Private;
    Private *d;
};

#endif // QABSTRACTLEAPMODEL_H
