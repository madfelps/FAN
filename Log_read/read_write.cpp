
#include "lerCAN.h"
/*Funções para printar no txt de cada parametro de leitura*/
void print_txt();
void if_motor_position();
void if_torque_timer();
void if_temperature_1();
void if_temperature_2();
void if_temperatures_3_torque_shudder();
void if_analog_input_voltages();
void if_internal_states();
void if_digital_input_status();
void if_current_information();
void if_voltage_information();
void if_flux_information();
void if_internal_voltages();
void if_fault_codes();

using namespace std;

string auxstr; // string que recebe os valores lidos do txt
string info;   //string que recebe a tradução dos valores de cada parâmetro. Exemplo: commanded_torque_mode = 0 -> info = "via CAN"
int wordCounter = 0; 
int frame_can[9];
int size;

ofstream file_CAN_processed("CAN_processed.txt"); //txt processado a ser preenchido
ifstream file_CAN("can.txt");                     //txt da CAN


/*0x0A0 – Temperatures #1*/
int module_a_temperature = 0;
int module_b_temperature = 0;
int module_c_temperature = 0;
int gate_driver_board_temperature = 0;
/*0x0A1 – Temperatures #2*/
int Control_Board_Temperature = 0;
int RTD_1_Temperature = 0;
int RTD_2_Temperature = 0;
int RTD_3_Temperature = 0;
/*0x0A2 – Temperatures #3 & Torque Shudder*/
int RTD_4_Temperature = 0;
int RTD_5_Temperature = 0;
int Motor_Temperature = 0;
int Torque_Shudder = 0;
/*0x0A3 – Analog Input Voltages*/
int analog_input_1 = 0;
int analog_input_2 = 0;
int analog_input_3 = 0;
int analog_input_4 = 0;
int analog_input_5 = 0;
int analog_input_6 = 0;
/*0x0A4 – Digital Input Status*/
bool digital_input_1 = 0;
bool digital_input_2 = 0;
bool digital_input_3 = 0;
bool digital_input_4 = 0;
bool digital_input_5 = 0;
bool digital_input_6 = 0;
bool digital_input_7 = 0;
bool digital_input_8 = 0;
/*0x0A5 – Motor Position Information*/
int motor_angle = 0;
int motor_speed = 0;
int elec_out_freq = 0;
int delta_reso_filtered = 0;
/*0x0A6 – Current Information*/
int Phase_A_Current = 0;
int Phase_B_Current = 0;
int Phase_C_Current = 0;
int DC_Bus_Current = 0;
/*Variáveis do TorqueTimerInfo*/
int commanded_torque = 0;
int torque_feedback = 0;
int power_on_timer = 0;
/*0x0A7 – Voltage Information*/
int DC_Bus_Voltage = 0;
int Output_Voltage = 0;
int VAB_Vd_Voltage = 0;
int VBC_Vq_Voltage = 0;
/*0xA8 – Flux Information*/
int Flux_command = 0;
int Flux_feedback = 0;
int Id_feedback = 0;
int Iq_feedback = 0;
/*0x0A9 – Internal Voltages*/
int Reference_voltage_1_5V = 0;
int Reference_voltage_2_5V = 0;
int Reference_voltage_5_0V = 0;
int System_voltage_12V = 0;
/*0x0AA – Internal States*/
int VSM_State = 0;
int Inverter_State = 0;
int Relay_State = 0;
int Inverter_Run_Mode = 0;
int Inverter_Active_Discharge_State = 0;
int Inverter_Command_Mode = 0;
int Inverter_Enable_State = 0;
int Inverter_Enable_Lockout = 0;
int Direction_Command = 0;
int BMS_Active = 0;
int BMS_Limiting_Torque = 0;
/*0x0AB – Fault Codes*/
int POST_Fault_Lo = 0;
int POST_Fault_Hi = 0;
int Run_Fault_Lo = 0;
int Run_Fault_Hi = 0;


int main()
{
    int count = 0;
    /*struct can_frame frameRead, frameWrite; //Criação dos frames
	frameRead.can_dlc = 8;*/

    /*TorqueTimerInfo ObjTorqueTimerInfo;
    MotorPosInfo ObjMotorPosInfo;
    Temperature1 ObjTemperature1;
    CommandMessage ObjCommandMessage;
    InternalStates ObjInternalStates;*/

    while (file_CAN >> auxstr)
    {
        /*Ignora "can1" e "can0" do txt*/
        if(auxstr == "can1" || auxstr == "can0")
        {
            //nothing
        }
        /*Verifica o tamanho da mensagem enviada pela CAN disponpivel no txt na forma [size]*/
        else if(auxstr == "[4]") size = 5;
        else if(auxstr == "[8]") size = 9;
        else if(auxstr == "[1]") size = 2;
        /*Faz a leitura do restante da linha do txt*/
        else
        {
        frame_can[wordCounter] = stoi(auxstr, nullptr, 16);
       
        cout <<frame_can[wordCounter] << " ";
        
        print_txt();
        wordCounter++;
        
        if (wordCounter == size)
        {
            wordCounter = 0; //finaliza uma linha do txt
            cout << endl;
        }
        }
        /*ObjMotorPosInfo.IfID_MotorPosInfo(&frameRead, UDP_Package);
        ObjTorqueTimerInfo.IfID_TorqueTimerInfo(&frameRead, UDP_Package);
        ObjTemperature1.IfID_Temperature1(&frameRead, UDP_Package);
        ObjInternalStates.IfID_InternalStates(&frameRead, UDP_Package);*/
    }
    /*Fecha os arquivos*/
    file_CAN.close();
    file_CAN_processed.close();

    return 0;
}

void print_txt()
{
    /*Verifica se o byte é de ID*/
    if (wordCounter == 0) 
    {
        if (frame_can[wordCounter] == 160)
            file_CAN_processed << "Temperature_1_Info: "<< endl << endl;
        if(frame_can[wordCounter] == 161)
            file_CAN_processed << "Temperature_2_Info: "<< endl << endl;
        if(frame_can[wordCounter] == 162)
            file_CAN_processed << "Temperatures_3_Torque_Shudder: "<< endl << endl;
        if(frame_can[wordCounter] == 163)
            file_CAN_processed << "Analog Input Voltages: "<< endl << endl;
        if(frame_can[wordCounter] == 164)
            file_CAN_processed << "Digital_Input_Status: "<< endl << endl;
        if (frame_can[wordCounter] == 165)
            file_CAN_processed << "Motor_Position_Info: " << endl << endl;
        if(frame_can[wordCounter] == 166)
            file_CAN_processed << "Current_Information: "<< endl << endl;
        if(frame_can[wordCounter] == 167)
            file_CAN_processed << "Voltage_Information: "<< endl << endl;
        if(frame_can[wordCounter] == 168)
            file_CAN_processed << "FLux_Information: "<< endl << endl;    
        if(frame_can[wordCounter] == 169)
            file_CAN_processed << "Internal_Voltages: "<< endl << endl;  
        if (frame_can[wordCounter] == 170)
            file_CAN_processed << "Internal_States: " << endl << endl;
        if (frame_can[wordCounter] == 172)
            file_CAN_processed << "Torque_Timer_Info: "<< endl << endl;
        
    }
    else
    {
        if_temperature_1();
        if_temperature_2();
        if_motor_position();
        if_torque_timer();
        if_temperatures_3_torque_shudder();
        if_analog_input_voltages();
        if_internal_states();
        if_digital_input_status();
        if_current_information();
        if_internal_voltages();
        if_voltage_information();
        if_flux_information();
        if_internal_voltages();
    } //fim do else
}

void if_temperature_1()
{

    if (frame_can[0] == 160)
    {
        if (wordCounter == 1) module_a_temperature += frame_can[1];
        else if (wordCounter == 2)
        {
            module_a_temperature += 256 * frame_can[2];
            file_CAN_processed << "IGBT_A_Temp:" << module_a_temperature << endl;
        }

        else if (wordCounter == 3)
            module_b_temperature += frame_can[3];
        else if (wordCounter == 4)
        {
            module_b_temperature += 256 * frame_can[4];
            file_CAN_processed << "IGBT_B_Temp:" << module_b_temperature << endl;
        }

        else if (wordCounter == 5)
            module_c_temperature += frame_can[5];
        else if (wordCounter == 6)
        {
            module_c_temperature += 256 * frame_can[6];
            file_CAN_processed << "IGBT_C_Temp:" << module_c_temperature << endl;
        }

        else if (wordCounter == 7)
            gate_driver_board_temperature += frame_can[7];
        else if (wordCounter == 8)
        {
            gate_driver_board_temperature += 256 * frame_can[8];
            file_CAN_processed << "Gate_Driver_Board_Temp:" << gate_driver_board_temperature << endl << "---------------------------"<<endl;
        }

    }
}
void if_temperature_2()
{
    if (frame_can[0] == 161)
    {
        if (wordCounter == 1) Control_Board_Temperature += frame_can[1];
        else if (wordCounter == 2)
        {
            Control_Board_Temperature += 256 * frame_can[2];
            file_CAN_processed << "Control_Board_Temperature:" << Control_Board_Temperature << endl;
        }

        else if (wordCounter == 3)
            RTD_1_Temperature += frame_can[3];
        else if (wordCounter == 4)
        {
            RTD_1_Temperature += 256 * frame_can[4];
            file_CAN_processed << "RTD_1_Temperature:" << RTD_1_Temperature << endl;
        }

        else if (wordCounter == 5)
            RTD_2_Temperature += frame_can[5];
        else if (wordCounter == 6)
        {
            RTD_2_Temperature += 256 * frame_can[6];
            file_CAN_processed << "RTD_2_Temperature:" << RTD_2_Temperature << endl;
        }

        else if (wordCounter == 7)
            RTD_3_Temperature += frame_can[7];
        else if (wordCounter == 8)
        {
            RTD_3_Temperature += 256 * frame_can[8];
            file_CAN_processed << "RTD_3_Temperature:" << RTD_3_Temperature << endl << "---------------------------"<<endl;
        }

    }
}
void if_temperatures_3_torque_shudder()
{
    if (frame_can[0] == 162)
    {
        if (wordCounter == 1) RTD_4_Temperature += frame_can[1];
        else if (wordCounter == 2)
        {
            RTD_4_Temperature += 256 * frame_can[2];
            file_CAN_processed << "RTD_4_Temperature:" << RTD_4_Temperature << endl;
        }

        else if (wordCounter == 3)
            RTD_5_Temperature += frame_can[3];
        else if (wordCounter == 4)
        {
            RTD_5_Temperature += 256 * frame_can[4];
            file_CAN_processed << "RTD_5_Temperature:" << RTD_5_Temperature << endl;
        }

        else if (wordCounter == 5)
            Motor_Temperature += frame_can[5];
        else if (wordCounter == 6)
        {
            Motor_Temperature += 256 * frame_can[6];
            file_CAN_processed << "Motor_Temperature:" << Motor_Temperature << endl;
        }

        else if (wordCounter == 7)
            Torque_Shudder += frame_can[7];
        else if (wordCounter == 8)
        {
            Torque_Shudder += 256 * frame_can[8];
            file_CAN_processed << "Torque_Shudder:" << Torque_Shudder << endl << "---------------------------"<<endl;
        }

    }
}
void if_analog_input_voltages()
{
    if (frame_can[0] == 163)
    {
        if (wordCounter == 1) analog_input_1 = frame_can[1] << 2; //0b1111111100 bits:0-7
        if (wordCounter == 2)
        {
            analog_input_1 += (frame_can[2] & 0b11000000)>>6; //+= (0b11111111 & 0b11000000)>>6 =+ (0b11000000)>>6 += 0b0000011 bits:8-9
            file_CAN_processed << "Analog_Input_1:" << analog_input_1 << endl;
        }
        if (wordCounter == 2) analog_input_2 += (frame_can[2] & 0b00111111)<<4; //=+ (0b111111111 & 0b00111111)<<4 +=(0b00111111)<<4 += 0b1111110000
        if (wordCounter == 3)
        {
            analog_input_2 += (frame_can[3] & 0b11110000)>>4; //+=(0b11111111 & 0b11110000)>>4 += (0b11110000)>>4 += 0b00001111
            file_CAN_processed << "Analog_Input_2:" << analog_input_2 << endl;
        }
        if (wordCounter == 3) analog_input_3 += (frame_can[3] & 0b00001111)<<6;// (0b11111111 & 0b00001111)<<6 = (0b00001111)<<6 = 0b1111000000
        if (wordCounter == 4)
        {
            analog_input_3 += (frame_can[4] & 0b11111100)>>2;// (0b11111111 & 0b11111100)>>2 = (0b11111100)>>2 = 0b00111111
            file_CAN_processed << "Analog_Input_3:" << analog_input_3 << endl;
        }
        if (wordCounter == 5) analog_input_4 += frame_can[5] << 2; //=
        if (wordCounter == 6)
        {
            analog_input_4 += (frame_can[6] & 0b11000000)>>6; // = 
            file_CAN_processed << "Analog_Input_4:" << analog_input_4 << endl;
        }
        if (wordCounter == 6) analog_input_5 += (frame_can[6] & 0b00111111) <<4; // = 
        if (wordCounter == 7)
        {
            analog_input_5 += (frame_can[7] & 0b11110000)>>4; // = 
            file_CAN_processed << "Analog_Input_5:" << analog_input_5 << endl;
        }
        if (wordCounter == 7) analog_input_6 += (frame_can[7] & 0b00001111)<<6; // 
        if (wordCounter == 8)
        {
            analog_input_6 += (frame_can[8] & 0b11111100)>>2; 
            file_CAN_processed << "Analog_Input_6:" << analog_input_6 << endl << "---------------------------"<<endl;
        }

    }
}
void if_digital_input_status()
{
    if (frame_can[0] == 164)
    {
        if(wordCounter == 1) 
        {
            digital_input_1 = frame_can[1];
            if(!digital_input_1) info = "Forward switch enabled";
            else info = "Forward switch disabled.";
            file_CAN_processed << "Digital_Input_1:" << info << endl;
        }
        else if(wordCounter == 2) 
        {
            digital_input_2 = frame_can[2];
            if(!digital_input_2) info = "Reverse switch enabled";
            else info = "Reverse switch disabled";
            file_CAN_processed << "Digital_Input_2:" << info << endl;
        }
        else if(wordCounter == 3) 
        {
            digital_input_3 = frame_can[3];
            if(!digital_input_3) info = "Brake switch enabled";
            else info = "Brake switch disabled";
            file_CAN_processed << "Digital_Input_3:" << info << endl;
        }
        else if(wordCounter == 4) 
        {
            digital_input_4 = frame_can[4];
            if(!digital_input_4) info =  "REGEN Disable Switch activated";
            else info = "REGEN Disable Switch disactivated";
            file_CAN_processed << "Digital_Input_4:" << info << endl;
        }
        else if(wordCounter == 5) 
        {
            digital_input_5 = frame_can[5];
            if(!digital_input_5) info =  "Ignition switch enabled";
            else info = "Ignition switch disabled";
            file_CAN_processed << "Digital_Input_5:" << info << endl;
        }
        else if(wordCounter == 6)
        {
            digital_input_6 = frame_can[6];
            if(!digital_input_6) info == "Start switch enabled";
            else info = "Start switch disabled";
            file_CAN_processed << "Digital_Input_6:" << info << endl;
        } 
        else if(wordCounter == 7)
        {
            digital_input_7 = frame_can[7];
            file_CAN_processed << "Digital_Input_7:" << digital_input_7 << endl;
        } 
        else if(wordCounter == 8)
        {
            digital_input_8 = frame_can[8];
            file_CAN_processed << "Digital_Input_8:" << digital_input_8 << endl << "---------------------------"<<endl;
        } 
    }
}
void if_motor_position()
{
    if (frame_can[0] == 165)
    {
        if (wordCounter == 1)
            motor_angle += frame_can[1];
        else if (wordCounter == 2)
        {
            motor_angle += 256 * frame_can[2];
            file_CAN_processed << "Motor_Speed:" << motor_angle << endl;
        }

        else if (wordCounter == 3)
            motor_speed += frame_can[3];
        else if (wordCounter == 4)
        {
            motor_speed += 256 * frame_can[4];
            file_CAN_processed << "Motor_Angle:" << motor_speed << endl;
        }

        else if (wordCounter == 5)
            elec_out_freq += frame_can[5];
        else if (wordCounter == 6)
        {
            elec_out_freq += 256 * frame_can[6];
            file_CAN_processed << "Elec_Out_Frequency:" << elec_out_freq << endl;
        }

        else if (wordCounter == 7)
            delta_reso_filtered += frame_can[7];
        else if (wordCounter == 8)
        {
            delta_reso_filtered += 256 * frame_can[8];
            file_CAN_processed << "Delta_Reso_FIltered:" << delta_reso_filtered << endl<< "---------------------------" << endl;
            
        }
    } 
}
void if_current_information()
{
    if (frame_can[0] == 166)
    {
        if (wordCounter == 1) Phase_A_Current += frame_can[1];
        else if (wordCounter == 2)
        {
            Phase_A_Current += 256 * frame_can[2];
            file_CAN_processed << "Phase_A_Current:" << Phase_A_Current << endl;
        }

        else if (wordCounter == 3)
            Phase_B_Current += frame_can[3];
        else if (wordCounter == 4)
        {
            Phase_B_Current += 256 * frame_can[4];
            file_CAN_processed << "Phase_B_Current:" << Phase_B_Current << endl;
        }

        else if (wordCounter == 5)
            Phase_C_Current += frame_can[5];
        else if (wordCounter == 6)
        {
            Phase_C_Current += 256 * frame_can[6];
            file_CAN_processed << "Phase_C_Current:" << Phase_C_Current << endl;
        }

        else if (wordCounter == 7)
            DC_Bus_Current += frame_can[7];
        else if (wordCounter == 8)
        {
            DC_Bus_Current += 256 * frame_can[8];
            file_CAN_processed << "DC_Bus_Current:" << Torque_Shudder << endl << "---------------------------"<<endl;
        }
    }
}
void if_voltage_information()
{
    if (frame_can[0] == 167)
    {
        if (wordCounter == 1) DC_Bus_Voltage += frame_can[1];
        else if (wordCounter == 2)
        {
            DC_Bus_Voltage += 256 * frame_can[2];
            file_CAN_processed << "DC_Bus_VOltage:" << DC_Bus_Voltage << endl;
        }

        else if (wordCounter == 3)
            Output_Voltage += frame_can[3];
        else if (wordCounter == 4)
        {
            Output_Voltage += 256 * frame_can[4];
            file_CAN_processed << "Output_Voltage:" << Output_Voltage << endl;
        }

        else if (wordCounter == 5)
            VAB_Vd_Voltage += frame_can[5];
        else if (wordCounter == 6)
        {
            VAB_Vd_Voltage += 256 * frame_can[6];
            file_CAN_processed << "VAB_Vd_Voltage:" << VAB_Vd_Voltage << endl;
        }

        else if (wordCounter == 7)
            VBC_Vq_Voltage+= frame_can[7];
        else if (wordCounter == 8)
        {
            VBC_Vq_Voltage += 256 * frame_can[8];
            file_CAN_processed << "VBC_Vq_Voltage:" << VBC_Vq_Voltage << endl << "---------------------------"<<endl;
        }
    }
}
void if_flux_information()
{
    if (frame_can[0] == 168)
    {
        if (wordCounter == 1) Flux_command += frame_can[1];
        else if (wordCounter == 2)
        {
            Flux_command += 256 * frame_can[2];
            file_CAN_processed << "Flux_Command:" << Flux_command << endl;
        }

        else if (wordCounter == 3)
            Flux_feedback += frame_can[3];
        else if (wordCounter == 4)
        {
            Flux_feedback += 256 * frame_can[4];
            file_CAN_processed << "Flux_feedback:" << Flux_feedback << endl;
        }

        else if (wordCounter == 5)
            Id_feedback+= frame_can[5];
        else if (wordCounter == 6)
        {
            Id_feedback+= 256 * frame_can[6];
            file_CAN_processed << "Id_feedback:" << Id_feedback << endl;
        }

        else if (wordCounter == 7)
            Iq_feedback += frame_can[7];
        else if (wordCounter == 8)
        {
            Iq_feedback += 256 * frame_can[8];
            file_CAN_processed << "Iq_feedback:" << Iq_feedback << endl << "---------------------------"<<endl;
        }
    }
}
void if_internal_voltages()
{
    if (frame_can[0] == 169)
    {
        if (wordCounter == 1) Reference_voltage_1_5V += frame_can[1];
        else if (wordCounter == 2)
        {
            Reference_voltage_1_5V += 256 * frame_can[2];
            file_CAN_processed << "1.5V_Reference_Voltage:" << Reference_voltage_1_5V << endl;
        }

        else if (wordCounter == 3)
            Reference_voltage_2_5V += frame_can[3];
        else if (wordCounter == 4)
        {
            Reference_voltage_2_5V += 256 * frame_can[4];
            file_CAN_processed << "2.5V_Reference_Voltage:" << Reference_voltage_2_5V << endl;
        }

        else if (wordCounter == 5)
            Reference_voltage_5_0V += frame_can[5];
        else if (wordCounter == 6)
        {
            Reference_voltage_5_0V += 256 * frame_can[6];
            file_CAN_processed << "5.0_Reference_Voltage:" << Reference_voltage_5_0V << endl;
        }

        else if (wordCounter == 7)
        System_voltage_12V += frame_can[7];
        else if (wordCounter == 8)
        {
            System_voltage_12V += 256 * frame_can[8];
            file_CAN_processed << "12V_System_Voltage:" << System_voltage_12V << endl << "---------------------------"<<endl;
        }
    }
}
void if_internal_states()
{

    if (frame_can[0] == 170)
    {
        if (wordCounter == 1) VSM_State += frame_can[1];
        else if (wordCounter == 2)
        {
            VSM_State +=  256*frame_can[2];
            switch(VSM_State)
            {
                case 0: info = "VSM Start State";
                case 1: info = "Pre-charge Init State";
                case 2: info = "Pre-charge Active State";
                case 3: info = "Pre-charge Complete State";
                case 4: info = "VSM Wait State";
                case 5: info = "VSM Ready State";
                case 6: info = "Motor Running State";
                case 7: info = "Blink Fault Code State";
                case 14: info = "Shutdown in Process";
                case 15: info = "Recycle Power State";
                
            }
            file_CAN_processed << "VSM_State:" << info << endl;
        }

        else if (wordCounter == 3) 
        {
            Inverter_State = frame_can[3];
            switch(Inverter_State)
            {
                case 0: info = "Power on State";
                case 1: info = "Stop State";
                case 2: info = "Open Loop State";
                case 3: info = "Closed Loop State";
                case 4: info = "Wait State";
                case 5: info = "Internal states";
                case 6: info = "Internal states";
                case 7: info = "Internal states";
                case 8: info = "Idle Run State";
                case 9: info = "Idle Stop State";
                case 10: info = "Internal states";
                
            }
            file_CAN_processed << "Inv_State:" << info << endl;
        }
        else if (wordCounter == 4)
        {
            Relay_State += frame_can[4];
            file_CAN_processed << "Relay_State:" << Relay_State << endl;
        }

        else if (wordCounter == 5)
        {
            Inverter_Run_Mode = 0b00000001 & frame_can[5]; //bit 0
            if(Inverter_Run_Mode == 0) info = "Torque Mode";
            else info = "Speed Mode";
            file_CAN_processed << "Inv_Run_Mode:" << info << endl;
            Inverter_Active_Discharge_State = 0b11100000 & frame_can[5];//bit 5-7
            switch(Inverter_Active_Discharge_State)
            {
                case 0: info = "Discharge Disabled";
                case 1: info = "Discharge Enabled, waiting";
                case 2: info = "Performing Speed Check";
                case 3: info = "Discharge Actively occurring";
                case 4: info = "Discharge Completed";
    
            }
            file_CAN_processed <<"Inv_Act_Disc_State:" << info << endl;
        } 
        else if (wordCounter == 6)
        {
            
            Inverter_Command_Mode = frame_can[6];
            if(Inverter_Command_Mode == 0) info = "CAN Mode";
            else info = "VSM Mode";
            file_CAN_processed << "Inverter_Command_Mode" << info << endl;

        }

        else if (wordCounter == 7)
        {
            Inverter_Enable_State = 0b00000001 & frame_can[7];
            if(Inverter_Enable_State == 0) info = "Inverter is disabled";
            else info = "Inverter is enabled";
            file_CAN_processed << "Inverter_Enable_State:" << info <<endl;
            Inverter_Enable_Lockout = 0b10000000 & frame_can[7];
            if(Inverter_Enable_Lockout == 0) info = "Inverter can be enabled";
            else info = "Inverter cannot be enabled";
            file_CAN_processed << "Inverter_Enable_Lock:"<<info<< endl;
        }
        else if (wordCounter == 8)
        {
            Direction_Command = 0b00000001 & frame_can[8];
            if(Direction_Command == 0) info = "Forward";
            else
            {
                if(Inverter_Enable_State == 1) info = "Reverse";
                else info = "Stopped";
            }
            file_CAN_processed << "Direction Command:"<<info<< endl;
            BMS_Active = 0b00000010 & frame_can[8];
            if(BMS_Active == 0) info = "BMS Message is not being received";
            else info = "BMS Message is being received";
            file_CAN_processed << "BMS Active:"<<info<< endl;
            BMS_Limiting_Torque = 0b00000100 & frame_can[8];
            if(BMS_Limiting_Torque == 0) info = "Torque is not being by the BMS";
            else info = "Torque is being limited by the BMS";
            file_CAN_processed << endl<<"---------------------------" << endl;
           
        }

    }
    
}



void if_torque_timer()
{

    if (frame_can[0] == 172)
    {
        if (wordCounter == 1) commanded_torque += frame_can[1];
        else if (wordCounter == 2)
        {
            commanded_torque += 256 * frame_can[2];
            file_CAN_processed << "Commanded_Torque:" << commanded_torque << endl;
        }

        else if (wordCounter == 3)  torque_feedback += frame_can[3];
        else if (wordCounter == 4)
        {
            torque_feedback += 256 * frame_can[4];
            file_CAN_processed << "Torque_Feedback:" << torque_feedback << endl;
        }

        else if (wordCounter == 5)
            power_on_timer += frame_can[5];
        else if (wordCounter == 6)
            power_on_timer += 256*frame_can[6];
        else if (wordCounter == 7)
            power_on_timer += 256*frame_can[7];
        else if (wordCounter == 8)
        {
            power_on_timer += 256*frame_can[8];
            file_CAN_processed << "Power_On_Timer:" << power_on_timer  << endl << "---------------------------"<<endl;
        }

    } //fim do if(frame_can[0] == 165)
}
