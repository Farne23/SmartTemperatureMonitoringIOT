package utils;

import java.time.LocalDate;

public class TemperatureSample {
	private final LocalDate date;
	private final double temperature;
	
	public TemperatureSample(LocalDate date, double temperature) {
		this.temperature = temperature;
		this.date = date;
	}
	

}
