package ru.igormayachenkov.flagman.radio

data class Message(
    val time : Long,
    val data : ByteArray
) {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Message

        if (time != other.time) return false
        if (!data.contentEquals(other.data)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = time.hashCode()
        result = 31 * result + data.contentHashCode()
        return result
    }
}
