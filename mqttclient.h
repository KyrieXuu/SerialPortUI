#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <QMqttClient>
#include <QMqttMessage>
#include <QJsonObject>
#include <QJsonDocument>


class MQTTClient
{
public:
    MQTTClient();
    ~MQTTClient();
    void publishJSONMessage(QString topic, QString msg, int qos);
    void connect(QString hostname, int port);
    void disconnect();

private:
    QMqttClient m_client;
};

#endif // MQTTCLIENT_H