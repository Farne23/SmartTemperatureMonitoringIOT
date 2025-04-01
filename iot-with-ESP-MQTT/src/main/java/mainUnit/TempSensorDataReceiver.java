package mainUnit;

import java.util.Date;

public interface TempSensorDataReceiver {
	void communicateTemperature(Date timestamp, double temperature);
}
