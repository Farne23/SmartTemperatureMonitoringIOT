package communication;

public interface TempMonitoringCommunicationClient {
	void setFrequency(int period);
	void getTemperatures();
}
