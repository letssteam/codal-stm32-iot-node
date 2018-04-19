/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University, UK.
Copyright (c) 2018 Paul ADAM, Europe.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

/**
  * Class definition for pressure.
  * Represents the pressure on the STM IOT node.
  */

#include "CodalConfig.h"
#include "STM32IotNode.h"
#include "STM32IotNodePressure.h"

namespace codal
{

/**
  * Constructor.
  *
  * Create a representation of the pressure on the STM32 IOT node
  *
  */
STM32IotNodePressure::STM32IotNodePressure( STM32IotNodeI2C& i2c )
: _i2c( i2c )
{
}

/**
 * Configures the pressure range defined
 * in this object. The nearest values are chosen to those defined
 * that are supported by the hardware. The instance variables are then
 * updated to reflect reality.
 *
 * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the pressure could not be configured.
 *
 * @note This method should be overidden by the hardware driver to implement the requested
 * changes in hardware.
 */

int STM32IotNodePressure::configure( )
{
 if ( !samplePeriod )
  samplePeriod = 1;
 float Value = 1000.0f / ( float ) samplePeriod;
 if ( ( ( HUMIDITY_Drv_t* ) DrvContext.pVTable )->Set_ODR_Value( &DrvContext, Value ) != COMPONENT_OK )
  return DEVICE_I2C_ERROR;
 if ( ( ( HUMIDITY_Drv_t* ) DrvContext.pVTable )->Get_ODR( &DrvContext, &Value ) != COMPONENT_OK )
  return DEVICE_I2C_ERROR;
 samplePeriod = 1000.0f / ( float ) Value;
 return DEVICE_OK;
}

/**
 * Poll to see if new data is available from the hardware. If so, update it.
 * n.b. it is not necessary to explicitly call this function to update data
 * (it normally happens in the background when the scheduler is idle), but a check is performed
 * if the user explicitly requests up to date data.
 *
 * @return DEVICE_OK on success, DEVICE_I2C_ERROR if the update fails.
 *
 * @note This method should be overidden by the hardware driver to implement the requested
 * changes in hardware.
 */

int STM32IotNodePressure::requestUpdate()
{
 if ( !DrvContext.isInitialized )
 {
   ( ( HUMIDITY_Drv_t* ) DrvContext.pVTable )->Init( &DrvContext );
  STM32IotNodePressure::configure();
  ( ( HUMIDITY_Drv_t* ) DrvContext.pVTable )->Sensor_Enable( &DrvContext );
 }
 float Data;
 if ( ( ( HUMIDITY_Drv_t* ) DrvContext.pVTable )->Get_Hum( &DrvContext, &Data ) == COMPONENT_OK )
 {
  sample = ( uint16_t ) ( Data );
  return DEVICE_OK;
 }
 return DEVICE_I2C_ERROR;
}

}