package utils;

public enum SystemState {
	NORMAL,HOT,TOO_HOT;
	
	public static SystemState reset() {
		return NORMAL;
	}
}
