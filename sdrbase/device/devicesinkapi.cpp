///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2016 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#include <plugin/plugininstancegui.h>
#include "device/devicesinkapi.h"
#include "device/devicesourceapi.h"
#include "dsp/devicesamplesink.h"
#include "plugin/plugininterface.h"
#include "settings/preset.h"
#include "dsp/dspengine.h"
#include "channel/channelsourceapi.h"

DeviceSinkAPI::DeviceSinkAPI(int deviceTabIndex,
        DSPDeviceSinkEngine *deviceSinkEngine) :
    m_deviceTabIndex(deviceTabIndex),
    m_deviceSinkEngine(deviceSinkEngine),
    m_sampleSinkSequence(0),
    m_nbItems(1),
    m_itemIndex(0),
    m_pluginInterface(0),
    m_sampleSinkPluginInstanceUI(0),
    m_buddySharedPtr(0),
    m_isBuddyLeader(false),
    m_masterTimer(DSPEngine::instance()->getMasterTimer())
{
}

DeviceSinkAPI::~DeviceSinkAPI()
{
}

void DeviceSinkAPI::addSpectrumSink(BasebandSampleSink *spectrumSink)
{
    m_deviceSinkEngine->addSpectrumSink(spectrumSink);
}

void DeviceSinkAPI::removeSpectrumSink(BasebandSampleSink* spectrumSink)
{
    m_deviceSinkEngine->removeSpectrumSink(spectrumSink);
}

void DeviceSinkAPI::addSource(BasebandSampleSource *source)
{
    m_deviceSinkEngine->addSource(source);
}

void DeviceSinkAPI::removeSource(BasebandSampleSource* source)
{
    m_deviceSinkEngine->removeSource(source);
}

void DeviceSinkAPI::addThreadedSource(ThreadedBasebandSampleSource* source)
{
    m_deviceSinkEngine->addThreadedSource(source);
}

void DeviceSinkAPI::removeThreadedSource(ThreadedBasebandSampleSource* source)
{
    m_deviceSinkEngine->removeThreadedSource(source);
}

void DeviceSinkAPI::addChannelAPI(ChannelSourceAPI* channelAPI)
{
    m_channelAPIs.append(channelAPI);
    renumerateChannels();
}

void DeviceSinkAPI::removeChannelAPI(ChannelSourceAPI* channelAPI)
{
    if (m_channelAPIs.removeOne(channelAPI)) {
        renumerateChannels();
    }

    channelAPI->setIndexInDeviceSet(-1);
}

ChannelSourceAPI *DeviceSinkAPI::getChanelAPIAt(int index)
{
    if (index < m_channelAPIs.size()) {
        return m_channelAPIs.at(index);
    } else {
        return 0;
    }
}

void DeviceSinkAPI::renumerateChannels()
{
    for (int i = 0; i < m_channelAPIs.size(); ++i)
    {
        m_channelAPIs.at(i)->setIndexInDeviceSet(i);
        m_channelAPIs.at(i)->setDeviceSetIndex(m_deviceTabIndex);
    }
}


uint32_t DeviceSinkAPI::getNumberOfSources()
{
    return m_deviceSinkEngine->getNumberOfSources();
}

void DeviceSinkAPI::setSampleSink(DeviceSampleSink* sink)
{
    m_deviceSinkEngine->setSink(sink);
}

DeviceSampleSink *DeviceSinkAPI::getSampleSink()
{
    return m_deviceSinkEngine->getSink();
}

bool DeviceSinkAPI::initGeneration()
{
    return m_deviceSinkEngine->initGeneration();
}

bool DeviceSinkAPI::startGeneration()
{
    return m_deviceSinkEngine->startGeneration();
}

void DeviceSinkAPI::stopGeneration()
{
    m_deviceSinkEngine->stopGeneration();
}
DSPDeviceSinkEngine::State DeviceSinkAPI::state() const
{
    return m_deviceSinkEngine->state();
}

QString DeviceSinkAPI::errorMessage()
{
    return m_deviceSinkEngine->errorMessage();
}

uint DeviceSinkAPI::getDeviceUID() const
{
    return m_deviceSinkEngine->getUID();
}

MessageQueue *DeviceSinkAPI::getDeviceEngineInputMessageQueue()
{
    return m_deviceSinkEngine->getInputMessageQueue();
}

MessageQueue *DeviceSinkAPI::getSampleSinkInputMessageQueue()
{
    return getSampleSink()->getInputMessageQueue();
}

MessageQueue *DeviceSinkAPI::getSampleSinkGUIMessageQueue()
{
    return getSampleSink()->getMessageQueueToGUI();
}

void DeviceSinkAPI::setHardwareId(const QString& id)
{
    m_hardwareId = id;
}

void DeviceSinkAPI::setSampleSinkId(const QString& id)
{
    m_sampleSinkId = id;
}

void DeviceSinkAPI::resetSampleSinkId()
{
    m_sampleSinkId.clear();
}

void DeviceSinkAPI::setSampleSinkSerial(const QString& serial)
{
    m_sampleSinkSerial = serial;
}

void DeviceSinkAPI::setSampleSinkDisplayName(const QString& name)
{
    m_sampleSinkDisplayName = name;
}

void DeviceSinkAPI::setSampleSinkSequence(int sequence)
{
    m_sampleSinkSequence = sequence;
    m_deviceSinkEngine->setSinkSequence(sequence);
}

void DeviceSinkAPI::setNbItems(uint32_t nbItems)
{
    m_nbItems = nbItems;
}

void DeviceSinkAPI::setItemIndex(uint32_t index)
{
    m_itemIndex = index;
}

void DeviceSinkAPI::setSampleSinkPluginInterface(PluginInterface *iface)
{
    m_pluginInterface = iface;
}

void DeviceSinkAPI::setSampleSinkPluginInstanceUI(PluginInstanceGUI *gui)
{
    m_sampleSinkPluginInstanceUI = gui;
}

void DeviceSinkAPI::getDeviceEngineStateStr(QString& state)
{
    if (m_deviceSinkEngine)
    {
        switch(m_deviceSinkEngine->state())
        {
        case DSPDeviceSinkEngine::StNotStarted:
            state = "notStarted";
            break;
        case DSPDeviceSinkEngine::StIdle:
            state = "idle";
            break;
        case DSPDeviceSinkEngine::StReady:
            state = "ready";
            break;
        case DSPDeviceSinkEngine::StRunning:
            state = "running";
            break;
        case DSPDeviceSinkEngine::StError:
            state = "error";
            break;
        default:
            state = "notStarted";
            break;
        }
    }
    else
    {
        state = "notStarted";
    }
}

void DeviceSinkAPI::loadSinkSettings(const Preset* preset)
{
    if (preset->isSourcePreset())
    {
        qDebug("DeviceSinkAPI::loadSinkSettings: Preset [%s | %s] is not a sink preset", qPrintable(preset->getGroup()), qPrintable(preset->getDescription()));
    }
    else
    {
        qDebug("DeviceSinkAPI::loadSinkSettings: Loading preset [%s | %s]", qPrintable(preset->getGroup()), qPrintable(preset->getDescription()));

        const QByteArray* sinkConfig = preset->findBestDeviceConfig(m_sampleSinkId, m_sampleSinkSerial, m_sampleSinkSequence);
        qint64 centerFrequency = preset->getCenterFrequency();
        qDebug("DeviceSinkAPI::loadSinkSettings: center frequency: %llu Hz", centerFrequency);

        if (sinkConfig != 0)
        {
            qDebug("DeviceSinkAPI::loadSinkSettings: deserializing sink %s[%d]: %s", qPrintable(m_sampleSinkId), m_sampleSinkSequence, qPrintable(m_sampleSinkSerial));

            if (m_sampleSinkPluginInstanceUI != 0) // GUI flavor
            {
                m_sampleSinkPluginInstanceUI->deserialize(*sinkConfig);
                m_sampleSinkPluginInstanceUI->setCenterFrequency(centerFrequency);
            }
            else if (m_deviceSinkEngine->getSink() != 0) // Server flavor
            {
                m_deviceSinkEngine->getSink()->deserialize(*sinkConfig);
                m_deviceSinkEngine->getSink()->setCenterFrequency(centerFrequency);
            }
            else
            {
                qDebug("DeviceSinkAPI::loadSinkSettings: no sink");
            }
        }
        else
        {
            qDebug("DeviceSinkAPI::loadSinkSettings: sink %s[%d]: %s not found", qPrintable(m_sampleSinkId), m_sampleSinkSequence, qPrintable(m_sampleSinkSerial));
        }
    }
}

void DeviceSinkAPI::saveSinkSettings(Preset* preset)
{
    if (preset->isSourcePreset())
    {
        qDebug("DeviceSinkAPI::saveSinkSettings: not a sink preset");
    }
    else
    {
        qDebug("DeviceSinkAPI::saveSourceSettings: serializing sink %s[%d]: %s", qPrintable(m_sampleSinkId), m_sampleSinkSequence, qPrintable(m_sampleSinkSerial));

        if (m_sampleSinkPluginInstanceUI != 0) // GUI flavor
        {
            preset->addOrUpdateDeviceConfig(m_sampleSinkId, m_sampleSinkSerial, m_sampleSinkSequence, m_deviceSinkEngine->getSink()->serialize());
            preset->setCenterFrequency(m_deviceSinkEngine->getSink()->getCenterFrequency());
        }
        else if (m_deviceSinkEngine->getSink() != 0) // Server flavor
        {
            preset->addOrUpdateDeviceConfig(m_sampleSinkId, m_sampleSinkSerial, m_sampleSinkSequence, m_deviceSinkEngine->getSink()->serialize());
            preset->setCenterFrequency(m_deviceSinkEngine->getSink()->getCenterFrequency());
        }
        else
        {
            qDebug("DeviceSinkAPI::saveSinkSettings: no sink");
        }
    }
}

void DeviceSinkAPI::addSourceBuddy(DeviceSourceAPI* buddy)
{
    m_sourceBuddies.push_back(buddy);
    buddy->m_sinkBuddies.push_back(this);
    qDebug("DeviceSinkAPI::addSourceBuddy: added buddy %s(%s) to the list [%llu] <-> [%llu]",
            qPrintable(buddy->getHardwareId()),
            qPrintable(buddy->getSampleSourceSerial()),
            (quint64) buddy,
            (quint64) this);
}

void DeviceSinkAPI::addSinkBuddy(DeviceSinkAPI* buddy)
{
    m_sinkBuddies.push_back(buddy);
    buddy->m_sinkBuddies.push_back(this);
    qDebug("DeviceSinkAPI::addSinkBuddy: added buddy %s(%s) to the list [%llu] <-> [%llu]",
            qPrintable(buddy->getHardwareId()),
            qPrintable(buddy->getSampleSinkSerial()),
            (quint64) buddy,
            (quint64) this);
}

void DeviceSinkAPI::removeSourceBuddy(DeviceSourceAPI* buddy)
{
    std::vector<DeviceSourceAPI*>::iterator it = m_sourceBuddies.begin();

    for (;it != m_sourceBuddies.end(); ++it)
    {
        if (*it == buddy)
        {
            qDebug("DeviceSinkAPI::removeSourceBuddy: buddy %s(%s) [%llu] removed from the list of [%llu]",
                    qPrintable(buddy->getHardwareId()),
                    qPrintable(buddy->getSampleSourceSerial()),
                    (quint64) (*it),
                    (quint64) this);
            m_sourceBuddies.erase(it);
            return;
        }
    }

    qDebug("DeviceSinkAPI::removeSourceBuddy: buddy %s(%s) [%llu] not found in the list of [%llu]",
            qPrintable(buddy->getHardwareId()),
            qPrintable(buddy->getSampleSourceSerial()),
            (quint64) buddy,
            (quint64) this);
}

void DeviceSinkAPI::removeSinkBuddy(DeviceSinkAPI* buddy)
{
    std::vector<DeviceSinkAPI*>::iterator it = m_sinkBuddies.begin();

    for (;it != m_sinkBuddies.end(); ++it)
    {
        if (*it == buddy)
        {
            qDebug("DeviceSinkAPI::removeSinkBuddy: buddy %s(%s) [%llu] removed from the list of [%llu]",
                    qPrintable(buddy->getHardwareId()),
                    qPrintable(buddy->getSampleSinkSerial()),
                    (quint64) (*it),
                    (quint64) this);
            m_sinkBuddies.erase(it);
            return;
        }
    }

    qDebug("DeviceSinkAPI::removeSinkBuddy: buddy %s(%s) [%llu] not found in the list of [%llu]",
            qPrintable(buddy->getHardwareId()),
            qPrintable(buddy->getSampleSinkSerial()),
            (quint64) buddy,
            (quint64) this);
}

void DeviceSinkAPI::clearBuddiesLists()
{
    std::vector<DeviceSourceAPI*>::iterator itSource = m_sourceBuddies.begin();
    std::vector<DeviceSinkAPI*>::iterator itSink = m_sinkBuddies.begin();
    bool leaderElected = false;

    for (;itSource != m_sourceBuddies.end(); ++itSource)
    {
        if (isBuddyLeader() && !leaderElected)
        {
            (*itSource)->setBuddyLeader(true);
            leaderElected = true;
        }

        (*itSource)->removeSinkBuddy(this);
    }

    m_sourceBuddies.clear();

    for (;itSink != m_sinkBuddies.end(); ++itSink)
    {
        if (isBuddyLeader() && !leaderElected)
        {
            (*itSink)->setBuddyLeader(true);
            leaderElected = true;
        }

        (*itSink)->removeSinkBuddy(this);
    }

    m_sinkBuddies.clear();
}

