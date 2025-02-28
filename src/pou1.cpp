
#include "pou_manager.h"

using namespace plc;

DiscreteInputArray<4> di_CPU (cpu_io);
DiscreteOutputArray<2> do_CPU (cpu_io);

AlphaXModule moduleDAIO(1);
AlphaXModule moduleAI(2);

DiscreteInputArray<5> di_DAIO(moduleDAIO);
DiscreteOutputArray<5> do_DAIO(moduleDAIO);
AnalogInputArray<2> ai_DAIO(moduleDAIO);

AnalogInputArray<8> ai_AI(moduleAI);

uint32_t i {0};

void _delay(uint32_t delay)
{
  uint32_t start = GetSysTicks();
  while (GetSysTicks() - start < delay) {
    // Можно добавить nop(); для того чтобы уменьшить нагрузку на процессор
  }
}

ModbusBuffer<1000> request_buffer;
ModbusBuffer<200> HMI_data;

ModbusMasterPoll poll(moduleDAIO, moduleAI);

ModbusMaster modbus_master ({
  .com        = COM1,         // Номер COM-порта
  .devices    = poll,      // Список устройств для авт.опроса
  .baudrate   = 115200,       // Скорость обмена, бит/с
  .parity     = COM_PARITY_NONE,  // Четность
  .stop_bits  = 1,            // Кол-во стоп-битов
  .poll_delay = 0,            // Пауза перед отправкой команды (мс)
  .response_timeout = 300,    // Время ожидания ответа (мс)
  .failed_attempts  = 3,      // Количество попыток связи
  .restore_timeout  = 3000    // Время восстановления опроса (мс)
});


ModbusMaster modbus_master2 ({
  // Номер COM-порта
  .com              = COM3,
  // Список устройств для автоматического опроса
  .devices          = {},
  // Скорость обмена, бит/с
  .baudrate         = 115200,
  // Четность
  .parity           = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits        = 1,
  // Пауза перед отправкой команды опроса (мс)
  .poll_delay       = 0,
  // Пауза после отправки широковещательного запроса (мс)
  .turnaround_delay = 100,
  // Время ожидания ответа (мс)
  .response_timeout = 300,
  // Количество попыток связи
  .failed_attempts  = 3,
  // Время восстановления опроса (мс)
  .restore_timeout  = 3000
});

ModbusSlave modbus_slave ({
  // Номер COM-порта
  .com               = COM2,
  // Таблица Holding Registers
  .holding_registers = &HMI_data,
  // Таблица Input Registers
  .input_registers   = &request_buffer,
  // Таблица Coils
  .coils             = nullptr,
  // Таблица Discrete Inputs
  .discrete_inputs   = nullptr,
  // Адрес Slave в сети Modbus
  .address           = 1,
  // Скорость обмена, бит/с
  .baudrate          = 115200,
  // Четность
  .parity            = COM_PARITY_NONE,
  // Кол-во стоп-битов
  .stop_bits         = 1,
  // Пауза перед отправкой ответа (мс)
  .poll_delay        = 0
});


ModbusMasterRequest request ({
  // Modbus Master для отправки запроса
  .master               = &modbus_master2,
  //! Адрес опрашиваемого устройства
  .device_address       = 3,
  // Номер функции Modbus
  .function             = 3,
  // Начальный адрес регистров Modbus в запросе
  .starting_address     = 0,
  // Указатель на таблицу регистров
  .buffer               = &request_buffer,
  // Общее кол-во регистров Modbus
  .registers_count      = 25,
  // Максимальное кол-во регистров Modbus в одном запросе к устройству
  .device_max_registers = 80,
});

etl::string<50> myString;
uint16_t pid_ai {0};


void POU1()
{
  static bool init = false;
  if (!init)
  {
    init = true;
    // modbus_master2.execute = true;
    // request.execute = true;
  }

  request.execute = true;
  if (request.done()) {
    request.execute = false;
    pid_ai = request_buffer.readUint16(0);
    
    
  }
  else if (request.error()) {
    request.execute = false;
    print_error("%d", request.error_id());
  }

  scan_debug(myString);
  if (!myString.empty())
  {
    // Расшифровка строки
    int16_t newValue;
    char str[20];
    sscanf(myString.data(), "%s %hd", str, &newValue);
    myString.assign(str);

    if (myString == "CPU" || myString == "cpu")
    {
             if (newValue == 1){ do_CPU[0] = !do_CPU[0].value();
      } else if (newValue == 2){ do_CPU[1] = !do_CPU[1].value();
      }
    }
    else if (myString == "DAIO" || myString == "daio")
    {
             if (newValue == 1){ do_DAIO[0] = !do_DAIO[0].value();
      } else if (newValue == 2){ do_DAIO[1] = !do_DAIO[1].value();
      } else if (newValue == 3){ do_DAIO[2] = !do_DAIO[2].value();
      } else if (newValue == 4){ do_DAIO[3] = !do_DAIO[3].value();
      } else if (newValue == 5){ do_DAIO[4] = !do_DAIO[4].value();
      }

      print_debug("DO5 [%d]\r\n", do_DAIO[4].value());
    }
    else if (myString == "print")
    {
      etl::string<32> analog_data;
      etl::string_stream stream{analog_data};

      print_debug("                T = %d.%d С\r\n", pid_ai/10, pid_ai%10);

      stream << etl::setprecision(2) << ai_AI[0].value() << "\r\n";
      print_debug("Канал температуры = ");
      print_debug(stream.str());
      analog_data.clear();

      stream << etl::setprecision(2) << ai_AI[1].value() << "\r\n";
      print_debug("  Канал влажности = ");
      print_debug(stream.str());
      analog_data.clear();

      stream << etl::setprecision(2) << ai_AI[6].value() << "%\r\n";
      print_debug("      Реостат AI7 = ");
      print_debug(stream.str());
      analog_data.clear();

      stream << etl::setprecision(2) << ai_AI[7].value() << "%\r\n";
      print_debug("      Реостат AI8 = ");
      print_debug(stream.str());
      analog_data.clear();

      stream << etl::setprecision(2) << ai_DAIO[0].value() << "\r\n";
      print_debug(" Тепература печки = ");
      print_debug(stream.str());
      analog_data.clear();
      

      stream << etl::setprecision(2) << ai_DAIO[1].value() << " C\r\n";
      print_debug("     Реостат DAIO = ");
      print_debug(stream.str());
      analog_data.clear();
      

      // Выводит в терминал значение аналогового входа с 2 знаками после запятой
      
      
          
    }
    myString.clear();
  }

  if (di_DAIO[0].front()){ do_DAIO[0] = true;
  } else if (di_DAIO[1].front()){ do_DAIO[1] = true;
  } else if (di_DAIO[0].back()){ do_DAIO[0] = false;
  } else if (di_DAIO[1].back()){ do_DAIO[1] = false;
  } else if (di_DAIO[2].front()){ do_DAIO[2] = !do_DAIO[2].value();
  } else if (di_DAIO[3].front()){ 
    do_DAIO[3] = !do_DAIO[3].value();
    do_DAIO[4] = !do_DAIO[4].value();

  } else if (di_CPU[0].front()){ do_CPU[0] = true;
  } else if (di_CPU[0].back()){ do_CPU[0] = false;
  } else if (di_CPU[1].front()){ do_CPU[1] = true;
  } else if (di_CPU[1].back()){ do_CPU[1] = false;
  } 
  


  // print_debug("%d %d \r\n", HMI_data.readFloat(4), HMI_data.readFloat(6));
  // print_debug("%d %d \r\n", ai_AI[6].value(), ai_AI[7].value());

  HMI_data.writeFloat(0, ai_DAIO[0]);
  HMI_data.writeInt16(2, request_buffer.readUint16(0)/10);
  HMI_data.writeFloat(3, ai_AI[6].value());
  HMI_data.writeFloat(5, ai_AI[7].value());
  HMI_data.writeInt16(7, request_buffer.readUint16(1)/10);
  HMI_data.writeFloat(8, ai_DAIO[1].value());


  request();
}
