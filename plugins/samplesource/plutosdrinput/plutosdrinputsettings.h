///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef _PLUTOSDR_PLUTOSDRINPUTSETTINGS_H_
#define _PLUTOSDR_PLUTOSDRINPUTSETTINGS_H_

#include <QtGlobal>
#include <stdint.h>

struct PlutoSDRInputSettings {
	typedef enum {
		FC_POS_INFRA = 0,
		FC_POS_SUPRA,
		FC_POS_CENTER
	} fcPos_t;

    enum RFPath
    {
        RFPATH_A_BAL = 0,
        RFPATH_B_BAL,
        RFPATH_C_BAL,
        RFPATH_A_NEG,
        RFPATH_A_POS,
        RFPATH_B_NEG,
        RFPATH_B_POS,
        RFPATH_C_NEG,
        RFPATH_C_POS,
        RFPATH_TX1MON,
        RFPATH_TX2MON,
        RFPATH_TX3MON,
        RFPATH_END
    };

    typedef enum {
        GAIN_MANUAL,
        GAIN_AGC_SLOW,
        GAIN_AGC_FAST,
        GAIN_HYBRID,
        GAIN_END
    } GainMode;

    typedef enum {
        RATEGOV_NORMAL,
        RATEGOV_HIGHOSR,
        RATEGOV_END
    } RateGovernor;

    // global settings to be saved
	quint64 m_centerFrequency;
    quint64 m_devSampleRate;
    RateGovernor m_rateGovernor;
    // channel settings
    fcPos_t m_fcPos;
    qint32  m_LOppmTenths;
    bool    m_dcBlock;
    bool    m_iqCorrection;
    quint32 m_log2Decim;
    float   m_lpfBW;           //!< analog lowpass filter bandwidth (Hz)
    bool    m_lpfFIREnable;    //!< enable digital lowpass FIR filter
    float   m_lpfFIRBW;        //!< digital lowpass FIR filter bandwidth (Hz)
    quint32 m_lpfFIRlog2Decim; //!< digital lowpass FIR filter log2 of decimation factor (0..2)
    int     m_lpfFIRGain;      //!< digital lowpass FIR filter gain (dB)
    uint32_t m_gain;           //!< "hardware" gain
    RFPath  m_antennaPath;
    GainMode m_gainMode;


	PlutoSDRInputSettings();
	void resetToDefaults();
	QByteArray serialize() const;
	bool deserialize(const QByteArray& data);
	static void translateGovernor(RateGovernor gov, QString& s);
    static void translateRFPath(RFPath path, QString& s);
    static void translateGainMode(GainMode mod, QString& s);
};

#endif /* _PLUTOSDR_PLUTOSDRINPUTSETTINGS_H_ */