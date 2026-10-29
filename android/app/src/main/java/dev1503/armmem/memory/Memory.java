package dev1503.armmem.memory;

/* loaded from: classes.dex */
public class Memory {
    public static final int RANGE_ANONYMOUS = 6;
    public static final int RANGE_ASHMEM = 9;
    public static final int RANGE_CODE_APP = 7;
    public static final int RANGE_C_ALLOC = 3;
    public static final int RANGE_C_BSS = 5;
    public static final int RANGE_C_DATA = 4;
    public static final int RANGE_C_HEAP = 1;
    public static final int RANGE_JAVA_HEAP = 2;
    public static final int RANGE_OTHER = 10;
    public static final int RANGE_STACK = 8;

    public static dev1503.armmem.memory.MemoryValueSet searchDword(int pid, int value, int memoryRange) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchDword(pid, value, memoryRange);
        for (long address : addresses) {
            memoryValueSet.addDword(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDword(int value, int memoryRange) {
        return searchDword(dev1503.armmem.memory.JNI.getPid(), value, memoryRange);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDword(int pid, int value, long[] prevList) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchDword(pid, value, prevList);
        for (long address : addresses) {
            memoryValueSet.addDword(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDword(int value, long[] prevList) {
        return searchDword(dev1503.armmem.memory.JNI.getPid(), value, prevList);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchFloat(int pid, float value, float radius, int memoryRange) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchFloat(pid, value, radius, memoryRange);
        for (long address : addresses) {
            memoryValueSet.addFloat(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchFloat(float value, float radius, int memoryRange) {
        return searchFloat(dev1503.armmem.memory.JNI.getPid(), value, radius, memoryRange);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchFloat(int pid, float value, float radius, long[] prevList) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchFloat(pid, value, radius, prevList);
        for (long address : addresses) {
            memoryValueSet.addFloat(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchFloat(float value, float radius, long[] prevList) {
        return searchFloat(dev1503.armmem.memory.JNI.getPid(), value, radius, prevList);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDouble(int pid, double value, double radius, int memoryRange) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchDouble(pid, value, radius, memoryRange);
        for (long address : addresses) {
            double value2 = value;
            int pid2 = pid;
            memoryValueSet.addDouble(pid2, address, value2);
            pid = pid2;
            value = value2;
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDouble(double value, double radius, int memoryRange) {
        return searchDouble(dev1503.armmem.memory.JNI.getPid(), value, radius, memoryRange);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDouble(int pid, double value, double radius, long[] prevList) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchDouble(pid, value, radius, prevList);
        for (long address : addresses) {
            double value2 = value;
            int pid2 = pid;
            memoryValueSet.addDouble(pid2, address, value2);
            pid = pid2;
            value = value2;
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchDouble(double value, double radius, long[] prevList) {
        return searchDouble(dev1503.armmem.memory.JNI.getPid(), value, radius, prevList);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchQword(int pid, long value, int memoryRange) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchQword(pid, value, memoryRange);
        for (long address : addresses) {
            memoryValueSet.addQword(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchQword(long value, int memoryRange) {
        return searchQword(dev1503.armmem.memory.JNI.getPid(), value, memoryRange);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchQword(int pid, long value, long[] prevList) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchQword(pid, value, prevList);
        for (long address : addresses) {
            memoryValueSet.addQword(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchQword(long value, long[] prevList) {
        return searchQword(dev1503.armmem.memory.JNI.getPid(), value, prevList);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchByte(int pid, byte value, int memoryRange) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchByte(pid, value, memoryRange);
        for (long address : addresses) {
            memoryValueSet.addByte(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchByte(byte value, int memoryRange) {
        return searchByte(dev1503.armmem.memory.JNI.getPid(), value, memoryRange);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchByte(int pid, byte value, long[] prevList) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchByte(pid, value, prevList);
        for (long address : addresses) {
            memoryValueSet.addByte(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchByte(byte value, long[] prevList) {
        return searchByte(dev1503.armmem.memory.JNI.getPid(), value, prevList);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchWord(int pid, short value, int memoryRange) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchWord(pid, value, memoryRange);
        for (long address : addresses) {
            memoryValueSet.addWord(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchWord(short value, int memoryRange) {
        return searchWord(dev1503.armmem.memory.JNI.getPid(), value, memoryRange);
    }

    public static dev1503.armmem.memory.MemoryValueSet searchWord(int pid, short value, long[] prevList) {
        dev1503.armmem.memory.MemoryValueSet memoryValueSet = new dev1503.armmem.memory.MemoryValueSet();
        long[] addresses = dev1503.armmem.memory.JNI.searchWord(pid, value, prevList);
        for (long address : addresses) {
            memoryValueSet.addWord(pid, address, value);
        }
        return memoryValueSet;
    }

    public static dev1503.armmem.memory.MemoryValueSet searchWord(short value, long[] prevList) {
        return searchWord(dev1503.armmem.memory.JNI.getPid(), value, prevList);
    }

    public static void writeDword(int pid, long address, int value) {
        dev1503.armmem.memory.JNI.writeDword(pid, address, value);
    }

    public static void writeDword(long address, int value) {
        dev1503.armmem.memory.JNI.writeDword(address, value);
    }

    public static void writeFloat(int pid, long address, float value) {
        dev1503.armmem.memory.JNI.writeFloat(pid, address, value);
    }

    public static void writeFloat(long address, float value) {
        dev1503.armmem.memory.JNI.writeFloat(address, value);
    }

    public static void writeDouble(int pid, long address, double value) {
        dev1503.armmem.memory.JNI.writeDouble(pid, address, value);
    }

    public static void writeDouble(long address, double value) {
        dev1503.armmem.memory.JNI.writeDouble(address, value);
    }

    public static int openMemFile(int pid) {
        return dev1503.armmem.memory.JNI.openMemFile(pid);
    }

    public static int openMemFile() {
        return dev1503.armmem.memory.JNI.openMemFile(dev1503.armmem.memory.JNI.getPid());
    }

    public static void closeMemFile(int fd) {
        dev1503.armmem.memory.JNI.closeMemFile(fd);
    }

    public static int readDword(int pid, long address) {
        return dev1503.armmem.memory.JNI.readDword(pid, address);
    }

    public static int readDword(long address, int fd) {
        return dev1503.armmem.memory.JNI.readDword(address, fd);
    }

    public static int readDword(long address) {
        return dev1503.armmem.memory.JNI.readDword(address);
    }

    public static float readFloat(int pid, long address) {
        return dev1503.armmem.memory.JNI.readFloat(pid, address);
    }

    public static float readFloat(long address, int fd) {
        return dev1503.armmem.memory.JNI.readFloat(address, fd);
    }

    public static float readFloat(long address) {
        return dev1503.armmem.memory.JNI.readFloat(address);
    }

    public static double readDouble(int pid, long address) {
        return dev1503.armmem.memory.JNI.readDouble(pid, address);
    }

    public static double readDouble(long address, int fd) {
        return dev1503.armmem.memory.JNI.readDouble(address, fd);
    }

    public static double readDouble(long address) {
        return dev1503.armmem.memory.JNI.readDouble(address);
    }
}
