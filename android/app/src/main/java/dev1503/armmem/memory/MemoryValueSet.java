package dev1503.armmem.memory;

/* loaded from: classes.dex */
public class MemoryValueSet extends java.util.HashSet<dev1503.armmem.memory.MemoryValue> {
    @Override // java.util.HashSet, java.util.AbstractCollection, java.util.Collection, java.util.Set
    public boolean add(dev1503.armmem.memory.MemoryValue memoryValue) {
        return super.add(memoryValue);
    }

    public boolean addDword(int pid, long address, int value) {
        return add(new dev1503.armmem.memory.MemoryValue(pid, address).writeDword(value));
    }

    public boolean addFloat(int pid, long address, float value) {
        return add(new dev1503.armmem.memory.MemoryValue(pid, address).writeFloat(value));
    }

    public boolean addDouble(int pid, long address, double value) {
        return add(new dev1503.armmem.memory.MemoryValue(pid, address).writeDouble(value));
    }

    public boolean addQword(int pid, long address, long value) {
        return add(new dev1503.armmem.memory.MemoryValue(pid, address).writeQword(value));
    }

    public boolean addByte(int pid, long address, byte value) {
        return add(new dev1503.armmem.memory.MemoryValue(pid, address).writeByte(value));
    }

    public boolean addWord(int pid, long address, short value) {
        return add(new dev1503.armmem.memory.MemoryValue(pid, address).writeWord(value));
    }

    public boolean contains(dev1503.armmem.memory.MemoryValue memoryValue) {
        return super.contains((java.lang.Object) memoryValue);
    }

    public int getFirstPid() {
        return iterator().next().getPid();
    }

    public long[] getAddresses(int count) {
        int size = java.lang.Math.min(size(), count);
        long[] addresses = new long[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            addresses[i] = iterator.next().getAddress();
        }
        return addresses;
    }

    public long[] getAddresses() {
        return getAddresses(size());
    }

    public int[] getDwordValues(int count) {
        int size = java.lang.Math.min(size(), count);
        int fd = dev1503.armmem.memory.Memory.openMemFile(getFirstPid());
        int[] dwordValues = new int[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            dwordValues[i] = iterator.next().readDword(fd);
        }
        dev1503.armmem.memory.Memory.closeMemFile(fd);
        return dwordValues;
    }

    public int[] getDwordValues() {
        return getDwordValues(size());
    }

    public float[] getFloatValues(int count) {
        int size = java.lang.Math.min(size(), count);
        int fd = dev1503.armmem.memory.Memory.openMemFile(getFirstPid());
        float[] floatValues = new float[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            floatValues[i] = iterator.next().readFloat(fd);
        }
        dev1503.armmem.memory.Memory.closeMemFile(fd);
        return floatValues;
    }

    public float[] getFloatValues() {
        return getFloatValues(size());
    }

    public double[] getDoubleValues(int count) {
        int size = java.lang.Math.min(size(), count);
        int fd = dev1503.armmem.memory.Memory.openMemFile(getFirstPid());
        double[] doubleValues = new double[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            doubleValues[i] = iterator.next().readDouble(fd);
        }
        dev1503.armmem.memory.Memory.closeMemFile(fd);
        return doubleValues;
    }

    public double[] getDoubleValues() {
        return getDoubleValues(size());
    }

    public long[] getQwordValues(int count) {
        int size = java.lang.Math.min(size(), count);
        int fd = dev1503.armmem.memory.Memory.openMemFile(getFirstPid());
        long[] qwordValues = new long[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            qwordValues[i] = iterator.next().readQword(fd);
        }
        dev1503.armmem.memory.Memory.closeMemFile(fd);
        return qwordValues;
    }

    public long[] getQwordValues() {
        return getQwordValues(size());
    }

    public byte[] getByteValues(int count) {
        int size = java.lang.Math.min(size(), count);
        int fd = dev1503.armmem.memory.Memory.openMemFile(getFirstPid());
        byte[] byteValues = new byte[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            byteValues[i] = iterator.next().readByte(fd);
        }
        dev1503.armmem.memory.Memory.closeMemFile(fd);
        return byteValues;
    }

    public byte[] getByteValues() {
        return getByteValues(size());
    }

    public short[] getWordValues(int count) {
        int size = java.lang.Math.min(size(), count);
        int fd = dev1503.armmem.memory.Memory.openMemFile(getFirstPid());
        short[] wordValues = new short[size];
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        for (int i = 0; i < size; i++) {
            wordValues[i] = iterator.next().readWord(fd);
        }
        dev1503.armmem.memory.Memory.closeMemFile(fd);
        return wordValues;
    }

    public short[] getWordValues() {
        return getWordValues(size());
    }

    public dev1503.armmem.memory.MemoryValue get(int index) {
        java.util.Iterator<dev1503.armmem.memory.MemoryValue> iterator = iterator();
        if (index >= size()) {
            return null;
        }
        for (int i = 0; i < index; i++) {
            iterator.next();
        }
        return iterator.next();
    }

    public dev1503.armmem.memory.MemoryValueSet searchDword(int value) {
        return dev1503.armmem.memory.Memory.searchDword(getFirstPid(), value, getAddresses());
    }

    public dev1503.armmem.memory.MemoryValueSet searchFloat(float value, float radius) {
        return dev1503.armmem.memory.Memory.searchFloat(getFirstPid(), value, radius, getAddresses());
    }

    public dev1503.armmem.memory.MemoryValueSet searchDouble(double value, double radius) {
        return dev1503.armmem.memory.Memory.searchDouble(getFirstPid(), value, radius, getAddresses());
    }

    public dev1503.armmem.memory.MemoryValueSet searchQword(long value) {
        return dev1503.armmem.memory.Memory.searchQword(getFirstPid(), value, getAddresses());
    }

    public dev1503.armmem.memory.MemoryValueSet searchByte(byte value) {
        return dev1503.armmem.memory.Memory.searchByte(getFirstPid(), value, getAddresses());
    }

    public dev1503.armmem.memory.MemoryValueSet searchWord(short value) {
        return dev1503.armmem.memory.Memory.searchWord(getFirstPid(), value, getAddresses());
    }
}
