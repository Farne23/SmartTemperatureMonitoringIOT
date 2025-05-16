package utils;

public enum SystemState {
	NORMAL,HOT,TOO_HOT,ALARM;
	
	public static SystemState reset() {
		return NORMAL;
	}
	
	public SystemState gotHotter() {
        switch (this) {
            case NORMAL:
                return HOT;
            case HOT:
                return TOO_HOT;
            default:
                return this;
        }
    }
	
	public SystemState gotCooler() {
        switch (this) {
            case TOO_HOT:
                return HOT;
            case HOT:
                return NORMAL;
            default:
                return this;
        }
    }
}
