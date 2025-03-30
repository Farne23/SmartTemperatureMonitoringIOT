package mainUnit;

import java.util.Date;

public interface TempSensorDataReceiver {
	void communicateTemperature(int temperature, Date date);
}
