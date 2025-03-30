package communication;

public interface TempMonitoringCommunicationClient {
	void initialize();
	void sendFrequency();
	void getTemperatures();
}
