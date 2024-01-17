#include "mqttclient.h"


MQTTClient::MQTTClient()
{

}

MQTTClient::~MQTTClient(){
    m_client.disconnectFromHost();
}

void MQTTClient::publishJSONMessage(QString topic, QString msg, int qos){

    // 创建JSON对象并设置要发送的数据
    QJsonObject json;
    json["message"] = msg;

    // 将JSON转换为字节数组
    QJsonDocument doc(json);
    QByteArray jsonData = doc.toJson();

    if (m_client.state() == QMqttClient::Connected){
        m_client.publish(topic, jsonData, qos);
    }else{
        qDebug() << "还未连接";
        return;
    }

}

void MQTTClient::connect(QString hostname, int port){
    // 设置连接IP与端口
    m_client.setHostname(hostname);
    m_client.setPort(static_cast<quint16>(port));
    m_client.connectToHost();
    QObject::connect(&m_client, &QMqttClient::stateChanged, [&](QMqttClient::ClientState state) {
        if (state == QMqttClient::Connected)
        {
            qDebug() << "MQTT connection is established.";
        }
        else if (state == QMqttClient::Disconnected)
        {
            qDebug() << "MQTT connection is disconnected.";
            retryConnect();
        }
    });
}

void MQTTClient::disconnect(){
    m_client.disconnectFromHost();
}

void MQTTClient::retryConnect() {
    while(m_client.state() == QMqttClient::Disconnected){
        m_client.connectToHost();
        qDebug() << "Retrying MQTT connection...";
        currentReconnectAttempts++;
        if(currentReconnectAttempts > maxReconnectAttempts){
            currentReconnectAttempts = 0;
            qDebug() << "超过重连次数，重连失败";
            break;
        }
    }
}
