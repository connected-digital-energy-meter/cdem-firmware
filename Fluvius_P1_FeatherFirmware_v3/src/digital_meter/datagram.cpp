#include "datagram.h"

namespace SmartMeter {

    double Datagram::consumption_low_tarif(){
        return consumptionlowtarif;
    };
    void Datagram::consumption_low_tarif(double setvalue){
        consumptionlowtarif=setvalue;
    };

    double Datagram::consumption_high_tarif(){
        return consumptionhightarif;
    };
    void Datagram::consumption_high_tarif(double setvalue){
        consumptionhightarif=setvalue;
    };
        
    double Datagram::production_low_tarif(){
        return productionlowtarif;
    };
    void Datagram::production_low_tarif(double setvalue){
        productionlowtarif=setvalue;
    };

    double Datagram::production_high_tarif(){
        return productionhightarif;
    };
    void Datagram::production_high_tarif(double setvalue){
        productionhightarif=setvalue;
    };

    double Datagram::total_power_consumption(){
        return totalpowerconsumption;
    };
    void Datagram::total_power_consumption(double setvalue){
        totalpowerconsumption=setvalue;
    };
    
    double Datagram::total_power_production(){
        return totalpowerproduction;
    };
    void Datagram::total_power_production(double setvalue){
        totalpowerproduction=setvalue;
    };
    
    double Datagram::actual_voltage_l1(){
        return actualvoltagel1;
    };
    void Datagram::actual_voltage_l1(double setvalue){
        actualvoltagel1=setvalue;
    };
    
    double Datagram::actual_voltage_l2(){
        return actualvoltagel2;
    };
    void Datagram::actual_voltage_l2(double setvalue){
        actualvoltagel2=setvalue;
    };
    
    double Datagram::actual_voltage_l3(){
        return actualvoltagel3;
    };
    void Datagram::actual_voltage_l3(double setvalue){
        actualvoltagel3=setvalue;
    };
    
    double Datagram::actual_current_l1(){
        return actualcurrentl1;
    };
    void Datagram::actual_current_l1(double setvalue){
        actualcurrentl1=setvalue;
    };
    
    double Datagram::actual_current_l2(){
        return actualcurrentl2;
    };
    void Datagram::actual_current_l2(double setvalue){
        actualcurrentl2=setvalue;
    };
    
    double Datagram::actual_current_l3(){
        return actualcurrentl3;
    };
    void Datagram::actual_current_l3(double setvalue){
        actualcurrentl3=setvalue;
    };
    
    double Datagram::l1_power_consumption(){
        return l1powerconsumption;
    };
    void Datagram::l1_power_consumption(double setvalue){
        l1powerconsumption=setvalue;
    };
    
    double Datagram::l2_power_consumption(){
        return l2powerconsumption;
    };
    void Datagram::l2_power_consumption(double setvalue){
        l2powerconsumption=setvalue;
    };
    
    double Datagram::l3_power_consumption(){
        return l3powerconsumption;
    };
    void Datagram::l3_power_consumption(double setvalue){
        l3powerconsumption=setvalue;
    };
    
    double Datagram::l1_power_production(){
        return l1powerproduction;
    };
    void Datagram::l1_power_production(double setvalue){
        l1powerproduction=setvalue;
    };
    
    double Datagram::l2_power_production(){
        return l2powerproduction;
    };
    void Datagram::l2_power_production(double setvalue){
        l2powerproduction=setvalue;
    };
    
    double Datagram::l3_power_production(){
        return l3powerproduction;
    };
    void Datagram::l3_power_production(double setvalue){
        l3powerproduction=setvalue;
    };
    
    double Datagram::gas_meter_m3(){
        return gasmeterm3;
    };
    void Datagram::gas_meter_m3(double setvalue){
        gasmeterm3=setvalue;
    };
    
    double Datagram::water_meter_m3(){
        return watermeterm3;
    };
    void Datagram::water_meter_m3(double setvalue){
        watermeterm3=setvalue;
    };
    
    double Datagram::actual_tarif(){
        return actualtarif;
    };
    void Datagram::actual_tarif(double setvalue){
        actualtarif=setvalue;
    };

    String Datagram::to_string(){
        String output="";
        output+="\t Consumption low tarif = "+String(consumptionlowtarif)+"\n";
        output+="\t Consumption high tarif = "+String(consumptionhightarif)+"\n";
        output+="\t Production low tarif = "+String(productionlowtarif)+"\n";
        output+="\t Production high tarif = "+String(productionhightarif)+"\n";
        output+="\t Total Power Consumption = "+String(totalpowerconsumption)+"\n";
        output+="\t Total Power Production = "+String(totalpowerproduction)+"\n";
        output+="\t Total Power Production = "+String(totalpowerproduction)+"\n";
        output+="\t Actual Voltage L1 = "+String(actualvoltagel1)+"\n";
        output+="\t Actual Voltage L2 = "+String(actualvoltagel2)+"\n";
        output+="\t Actual Voltage L3 = "+String(actualvoltagel3)+"\n";
        output+="\t Actual Current L1 = "+String(actualcurrentl1)+"\n";
        output+="\t Actual Current L2 = "+String(actualcurrentl2)+"\n";
        output+="\t Actual Current L3 = "+String(actualcurrentl3)+"\n";
        output+="\t Power Consumption L1 = "+String(l1powerconsumption)+"\n";
        output+="\t Power Consumption L2 = "+String(l2powerconsumption)+"\n";
        output+="\t Power Consumption L3 = "+String(l3powerconsumption)+"\n";
        output+="\t Power Production L1 = "+String(l1powerproduction)+"\n";
        output+="\t Power Production L2 = "+String(l2powerproduction)+"\n";
        output+="\t Power Production L3 = "+String(l3powerproduction)+"\n";
        output+="\t Gas Meter = "+String(gasmeterm3)+"\n";
        output+="\t Water Meter = "+String(watermeterm3)+"\n";
        output+="\t Actual Tarif = "+String(actualtarif)+"\n";
        return output;
    }
}