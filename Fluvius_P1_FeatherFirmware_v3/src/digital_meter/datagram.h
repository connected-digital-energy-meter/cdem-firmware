#pragma once
#include <Arduino.h>

namespace SmartMeter {
  class Datagram {
      public:
        double consumption_low_tarif();
        void consumption_low_tarif(double setvalue);
        double consumption_high_tarif();
        void consumption_high_tarif(double setvalue);
        double production_low_tarif();
        void production_low_tarif(double setvalue);
        double production_high_tarif();
        void production_high_tarif(double setvalue);
        double total_power_consumption();
        void total_power_consumption(double setvalue);
        double total_power_production();
        void total_power_production(double setvalue);
        double actual_voltage_l1();
        void actual_voltage_l1(double setvalue);
        double actual_voltage_l2();
        void actual_voltage_l2(double setvalue);
        double actual_voltage_l3();
        void actual_voltage_l3(double setvalue);
        double actual_current_l1();
        void actual_current_l1(double setvalue);
        double actual_current_l2();
        void actual_current_l2(double setvalue);
        double actual_current_l3();
        void actual_current_l3(double setvalue);
        double l1_power_consumption();
        void l1_power_consumption(double setvalue);
        double l2_power_consumption();
        void l2_power_consumption(double setvalue);
        double l3_power_consumption();
        void l3_power_consumption(double setvalue);
        double l1_power_production();
        void l1_power_production(double setvalue);
        double l2_power_production();
        void l2_power_production(double setvalue);
        double l3_power_production();
        void l3_power_production(double setvalue);
        double gas_meter_m3();
        void gas_meter_m3(double setvalue);
        double water_meter_m3();
        void water_meter_m3(double setvalue);
        double actual_tarif();
        void actual_tarif(double setvalue);
        String to_string();
      private:
        double consumptionlowtarif=0;
        double consumptionhightarif=0;
        double productionlowtarif=0;
        double productionhightarif=0;
        double totalpowerconsumption=0;
        double totalpowerproduction=0;
        double actualvoltagel1=0;
        double actualvoltagel2=0;
        double actualvoltagel3=0;
        double actualcurrentl1=0;
        double actualcurrentl2=0;
        double actualcurrentl3=0;
        double l1powerconsumption=0;
        double l2powerconsumption=0;
        double l3powerconsumption=0;
        double l1powerproduction=0;
        double l2powerproduction=0;
        double l3powerproduction=0;
        double gasmeterm3=0;
        double watermeterm3=0;
        double actualtarif=0;
  };
};