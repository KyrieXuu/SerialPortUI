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
    void retryConnect();

private:
    QMqttClient m_client;
    // 设置最大重连次数
    int maxReconnectAttempts = 5;
    int currentReconnectAttempts = 0;
};

#endif // MQTTCLIENT_H
