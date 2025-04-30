package utils;

public enum ControlMode {
	AUTOMATIC,MANUAL;
	
	 public ControlMode switchMode() {
	        return this == AUTOMATIC ? MANUAL : AUTOMATIC;
	 }
}
