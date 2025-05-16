package utils;

import java.time.LocalDateTime;

public class TemperatureSample {
	private final LocalDateTime date;
	private final double temperature;
	
	public TemperatureSample(LocalDateTime date, double temperature) {
		this.temperature = temperature;
		this.date = date;
	}
	
	public LocalDateTime getDateTime() {
		return date;
	}
	
	public double getTemperature(){
		return temperature;
	}
}
