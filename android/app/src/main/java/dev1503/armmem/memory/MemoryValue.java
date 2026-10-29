package dev1503.armmem.memory;

/* loaded from: classes.dex */
public class MemoryValue {
    private long address;
    private int pid;

    public MemoryValue(int pid, long address) {
        this.pid = pid;
        this.address = address;
    }

    public long getAddress() {
        return this.address;
    }

    public int getPid() {
        return this.pid;
    }

    public dev1503.armmem.memory.MemoryValue setPid(int pid) {
        this.pid = pid;
        return this;
    }

    public dev1503.armmem.memory.MemoryValue setAddress(long address) {
        this.address = address;
        return this;
    }

    public dev1503.armmem.memory.MemoryValue writeDword(int value) {
        dev1503.armmem.memory.JNI.writeDword(this.pid, this.address, value);
        return this;
    }

    public dev1503.armmem.memory.MemoryValue writeFloat(float value) {
        dev1503.armmem.memory.JNI.writeFloat(this.pid, this.address, value);
        return this;
    }

    public dev1503.armmem.memory.MemoryValue writeDouble(double value) {
        dev1503.armmem.memory.JNI.writeDouble(this.pid, this.address, value);
        return this;
    }

    public dev1503.armmem.memory.MemoryValue writeQword(long value) {
        dev1503.armmem.memory.JNI.writeQword(this.pid, this.address, value);
        return this;
    }

    public dev1503.armmem.memory.MemoryValue writeByte(byte value) {
        dev1503.armmem.memory.JNI.writeByte(this.pid, this.address, value);
        return this;
    }

    public dev1503.armmem.memory.MemoryValue writeWord(short value) {
        dev1503.armmem.memory.JNI.writeWord(this.pid, this.address, value);
        return this;
    }

    public int readDword() {
        return dev1503.armmem.memory.JNI.readDword(this.pid, this.address);
    }

    public float readFloat() {
        return dev1503.armmem.memory.JNI.readFloat(this.pid, this.address);
    }

    public double readDouble() {
        return dev1503.armmem.memory.JNI.readDouble(this.pid, this.address);
    }

    public long readQword() {
        return dev1503.armmem.memory.JNI.readQword(this.pid, this.address);
    }

    public byte readByte() {
        return dev1503.armmem.memory.JNI.readByte(this.pid, this.address);
    }

    public short readWord() {
        return dev1503.armmem.memory.JNI.readWord(this.pid, this.address);
    }

    public int readDword(int fd) {
        return dev1503.armmem.memory.JNI.readDword(this.address, fd);
    }

    public float readFloat(int fd) {
        return dev1503.armmem.memory.JNI.readFloat(this.address, fd);
    }

    public double readDouble(int fd) {
        return dev1503.armmem.memory.JNI.readDouble(this.address, fd);
    }

    public long readQword(int fd) {
        return dev1503.armmem.memory.JNI.readQword(this.address, fd);
    }

    public byte readByte(int fd) {
        return dev1503.armmem.memory.JNI.readByte(this.address, fd);
    }

    public short readWord(int fd) {
        return dev1503.armmem.memory.JNI.readWord(this.address, fd);
    }
}
