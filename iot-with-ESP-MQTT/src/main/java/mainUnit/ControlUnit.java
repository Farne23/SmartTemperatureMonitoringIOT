package mainUnit;

import java.util.Stack;

import utils.TemperatureSample;

import java.util.Date;

/*
 * Class implementing the main functions of the Window Controller Subsystem,
 */
public class ControlUnit implements TempSensorDataReceiver {
	
	private static final int MAX_HISTORY_SAMPLES = 100;
	
	//Stack keeping track of the last N temperatures sampled
	private Stack<TemperatureSample> temperatures;
	
	public ControlUnit () {}

	@Override
	public void communicateTemperature(Date date, double temperature) {
		temperatures.push(new TemperatureSample(date,temperature));	
	}

}
