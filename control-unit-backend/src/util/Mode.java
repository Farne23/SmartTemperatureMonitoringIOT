package util;

public enum Mode {
    MANUAL("M"),
    AUTO("A");

    private final String code;

    private Mode(String code) {
        this.code = code;
    }

    public String getCode() {
        return this.code;
    }
}
