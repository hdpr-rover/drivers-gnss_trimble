/*    Robotic Construction Kit (ROCK) Driver  for the BD970 GNSS receiver 
 *    from Trimble.
 *    Copyright (C) 2014  Vassilios Tsounis
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *    
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    NMEATypes.cpp  Copyright (C) 2014  Vassilios Tsounis
 *    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
 *    This is free software, and you are welcome to redistribute it
 *    under certain conditions; type `show c' for details.
 */

/** NMEA Types Header **/
#include <trimble_bd970/NMEATypes.hpp>

/** Std C Libraries **/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/** Std C++ Libraries **/
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

/** Boost C++ Libraries **/
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

/** POSIX & GNU/Linux Libraries **/
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <termio.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>



using namespace std;
using namespace boost;
using namespace trimble_bd970;



/**
* 
*/
typedef vector< string > split_vector_type;



/**
* 
*/
NMEA_GGA::NMEA_GGA(void)
    : NMEA_Base(-1, 15)
    , utc(-1)
    , latitude(0)
    , lat_dir("~")
    , longtitude(0)
    , long_dir("~")
    , gps_quality(-1)
    , num_sat_vehicles(0)
    , hdop(0)
    , orthom_height(0)
    , geoid_separation(0)
    , dgps_age(-1)
    , ref_station_id(-1)
    , checksum("~")
{
}

NMEA_GGA::~NMEA_GGA(void)
{
}

int NMEA_GGA::extractMessage(uint8_t *buffer, int message_len)
{
    uint8_t *temp = new uint8_t[message_len - 1];
    memcpy(temp, buffer, (message_len - 1));
    temp[message_len - 1] = '\0';
    std::string input((char *)temp);
    delete temp;
    
    split_vector_type SplitVec;
    split( SplitVec, input, is_any_of(",*") );
        
    for(unsigned int i = 0; i < SplitVec.size(); ++i)
    {
        if(i == 0)
        {
            if(SplitVec[i].compare("$GPGGA") != 0)
            {
                std::runtime_error("NMEA_GGA: packet given is not GPGGA.");
            }
        }
        
        if(i == 1)
        {
            utc = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 2)
        {
            latitude = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 3)
        {
            lat_dir = SplitVec[i];
        }
        
        if(i == 4)
        {
            longtitude = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 5)
        {
            long_dir = SplitVec[i];
        }
        
        if(i == 6)
        {
            gps_quality = atoi(SplitVec[i].c_str());
        }
        
        if(i == 7)
        {
            num_sat_vehicles = atoi(SplitVec[i].c_str());
        }
        
        if(i == 8)
        {
            hdop = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 9)
        {
            orthom_height = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 11)
        {
            geoid_separation = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 13)
        {
            dgps_age = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 14)
        {
            ref_station_id = atoi(SplitVec[i].c_str());
        }
        
        if(i == 15)
        {
            //TODO: use: int i_hex = std::stoi (str_hex, 0,16);
            checksum = SplitVec[i];
        }
    }
    
    //printf("@%s, LINE: %d\n", __FILE__, __LINE__);
    return 0;
}

int NMEA_GGA::printMessage(void)
{
    std::cout << "These are the found values: " << std::endl;
    std::cout << std::endl;
    
    std::cout << "UTC: " << utc << std::endl;
    std::cout << "LAT: " << latitude << std::endl;
    std::cout << "LATDIR: " << lat_dir << std::endl;
    std::cout << "LONG: " << longtitude << std::endl;
    std::cout << "LONGDIR: " << long_dir << std::endl;
    std::cout << "GPS_QUAL: " << gps_quality << std::endl;
    std::cout << "SATs: " << num_sat_vehicles << std::endl;
    std::cout << "HDOP: " << hdop << std::endl;
    std::cout << "OH: " << orthom_height << std::endl;
    std::cout << "GS: " << geoid_separation << std::endl;
    std::cout << "DGPS AGE: " << dgps_age << std::endl;
    std::cout << "REFST: " << ref_station_id << std::endl;
    std::cout << "CHKSM: " << checksum << std::endl;
    std::cout << std::endl;
    
    return 0;
}


/**
* 
*/
NMEA_AVR::NMEA_AVR(void)
    : NMEA_Base(-1, 12)
    , utc(0)
    , yaw(0)
    , tilt(0)
    , range(0)
    , gps_quality(0)
    , pdop(0)
    , num_sat_vehicles(0)
    , checksum("~")
{
}

NMEA_AVR::~NMEA_AVR(void)
{
}

int NMEA_AVR::extractMessage(uint8_t *buffer, int message_len)
{
    uint8_t *temp = new uint8_t[message_len - 1];
    memcpy(temp, buffer, (message_len - 1));
    temp[message_len - 1] = '\0';
    std::string input((char *)temp);
    delete temp;
    
    split_vector_type SplitVec;
    split( SplitVec, input, is_any_of(",*") );
        
    for(unsigned int i = 0; i < SplitVec.size(); ++i)
    {
        if(i == 1)
        {
            if(SplitVec[i].compare("AVR") != 0)
            {
                std::runtime_error("NMEA_AVR: packet given is not AVR.");
            }
        }
        
        if (i == 2)
        {
            utc = strtod(SplitVec[i].c_str(), 0);
        }
        
        if (i == 3)
        {
            yaw = strtod(SplitVec[i].c_str(), 0);
        }
        
        if (i == 5)
        {
            tilt = strtod(SplitVec[i].c_str(), 0);
        }
        
        if (i == 9)
        {
            range = strtod(SplitVec[i].c_str(), 0);
        }
        
        if (i == 10)
        {
            gps_quality = atoi(SplitVec[i].c_str());
        }
        
        if (i == 12)
        {
            pdop = strtod(SplitVec[i].c_str(), 0);
        }
        
        if(i == 13)
        {
            //TODO: use: int i_hex = std::stoi (str_hex, 0,16);
            checksum = SplitVec[i];
        }
    }
    
    //printf("@%s, LINE: %d\n", __FILE__, __LINE__);
    return 0;
}

int NMEA_AVR::printMessage(void)
{
    std::cout << "These are the found values: " << std::endl;
    std::cout << std::endl;
    
    std::cout << "UTC: " << utc << std::endl;
    std::cout << "YAW: " << yaw << std::endl;
    std::cout << "TILT: " << tilt << std::endl;
    std::cout << "RANGE: " << range << std::endl;
    std::cout << "GPS_QUAL: " << gps_quality << std::endl;
    std::cout << "PDOP: " << pdop << std::endl;
    std::cout << "SATs: " << num_sat_vehicles << std::endl;
    std::cout << "CHKSM: " << checksum << std::endl;
    std::cout << std::endl;
    
    return 0;
}


/**
* 
*/
NMEA_HDT::NMEA_HDT(void)
    : NMEA_Base(-1, 3)
    , heading(0)
    , heading_dir("~")
    , checksum("~")
{
}

NMEA_HDT::~NMEA_HDT(void)
{
}

int NMEA_HDT::extractMessage(uint8_t *buffer, int message_len)
{
    uint8_t *temp = new uint8_t[message_len - 1];
    memcpy(temp, buffer, (message_len - 1));
    temp[message_len - 1] = '\0';
    std::string input((char *)temp);
    delete temp;
    
    split_vector_type SplitVec;
    split( SplitVec, input, is_any_of(",*") );
        
    for(unsigned int i = 0; i < SplitVec.size(); ++i)
    {
        if(i == 0)
        {
            if(SplitVec[i].compare("$GPHDT") != 0)
            {
                std::runtime_error("NMEA_HDT: packet given is not GPHDT.");
            }
        }
            
        if (i == 1)
        {
            heading = strtod(SplitVec[i].c_str(), 0);
        }
        
        if (i == 2)
        {
             heading_dir = SplitVec[i];
        }
        
        if(i == 3)
        {
            //TODO: use: int i_hex = std::stoi (str_hex, 0,16);
            checksum = SplitVec[i];
        }
    }
    
    //printf("@%s, LINE: %d\n", __FILE__, __LINE__);
    return 0;
}

int NMEA_HDT::printMessage(void)
{
    std::cout << "These are the found values: " << std::endl;
    std::cout << std::endl;
    
    std::cout << "HEADING: " << heading << std::endl;
    std::cout << "HDIR: " << heading_dir << std::endl;
    std::cout << "CHKSM: " << checksum << std::endl;
    std::cout << std::endl;
    
    return 0;
}


/**
* 
*/
NMEA_Messages::NMEA_Messages(void)
    : m_number_messages(NMEA_MESSAGE_NUM)
    , m_message_lengths(0)
    , data_gga()
    , data_avr()
    , data_hdt()
{
    /* TODO: ADD MESSAGES HERE */
    
    
    m_rx_time = base::Time::now();
    m_tx_time = base::Time::now();
    
    this->m_message_lengths = new int[NMEA_MESSAGE_NUM];
}

NMEA_Messages::~NMEA_Messages(void)
{
    /* TODO: ADD MESSAGES HERE */
    
    delete this->m_message_lengths;
}

int NMEA_Messages::extractNMEA(uint8_t *buffer)
{
    m_rx_time = base::Time::now();
    
    uint8_t *p_buffer = buffer;
    data_gga.extractMessage(p_buffer, m_message_lengths[0]);
    
    p_buffer += m_message_lengths[0];
    data_avr.extractMessage(p_buffer, m_message_lengths[1]);
    
    p_buffer += m_message_lengths[1];
    data_hdt.extractMessage(p_buffer, m_message_lengths[2]);
    
    m_tx_time = base::Time::now();
    
    return 0;
}

int NMEA_Messages::printMessages(void)
{
    data_gga.printMessage();
    data_avr.printMessage();
    data_hdt.printMessage();
    
    int64_t process_time = m_tx_time.toMicroseconds() - m_rx_time.toMicroseconds();
    
    std::cout << "NMEA processing time (usec) is: " << process_time << std::endl;
    
    return 0;
}