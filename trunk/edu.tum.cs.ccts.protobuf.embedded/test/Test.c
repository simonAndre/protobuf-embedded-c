/******************************************************************* 
 * C-file generated by Protoc for Embedded C.                      *
 * Version 0.2 (2011-###-##)                                       *
 *                                                                 *
 * Copyright (c) 2009-2011                                         *
 * Technische Universitaet Muenchen                                *
 * http://www4.in.tum.de/                                          *
 *                                                                 *
 * Source : Test.proto
 * Package: edu.tum.cs.ccts.protobuf.embedded
 *                                                                 *
 * Do not edit.                                                    *
 *******************************************************************/

#include "Test.h"

/*******************************************************************
 * General functions
 *******************************************************************/
 
void _memset(void *msg_ptr, char init_val, unsigned int size) {
    int i;
    for(i = 0; i < size; ++ i)
        *((char*)msg_ptr + i) = init_val;
}

int varint_packed_size(unsigned long value) {
    if ((value & (0xffffffff <<  7)) == 0) return 1;
    if ((value & (0xffffffff << 14)) == 0) return 2;
    if ((value & (0xffffffff << 21)) == 0) return 3;
    if ((value & (0xffffffff << 28)) == 0) return 4;
    return 5;  
}

int write_raw_byte(char value, void *_buffer, int offset) {
    *((char *)_buffer + offset) = value;
    return ++offset;
}

/** Write a little-endian 32-bit integer. */
int write_raw_little_endian32(unsigned long value, void *_buffer, int offset) {
    offset = write_raw_byte((char)((value      ) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >>  8) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 16) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 24) & 0xFF), _buffer, offset);
    
    return offset;
}

/** Write a little-endian 64-bit integer. */
int write_raw_little_endian64(unsigned long long value, void *_buffer, int offset) {
    offset = write_raw_byte((char)((value      ) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >>  8) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 16) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 24) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 32) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 40) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 48) & 0xFF), _buffer, offset);
    offset = write_raw_byte((char)((value >> 56) & 0xFF), _buffer, offset);
    
    return offset;
}

int write_raw_varint32(unsigned long value, void *_buffer, int offset) {
    while (1) {
        if ((value & ~0x7F) == 0) {
            offset = write_raw_byte((char)value, _buffer, offset);
            return offset;
        } else {
            offset = write_raw_byte((char)((value & 0x7F) | 0x80), _buffer, offset);
            value = value >> 7;
        }
    }
    return offset;
}

int write_raw_varint64(unsigned long long value, void *_buffer, int offset) {
    while (1) {
        if ((value & ~0x7FL) == 0) {
            offset = write_raw_byte((char)value, _buffer, offset);
            return offset;
        } else {
            offset = write_raw_byte((char)((value & 0x7F) | 0x80), _buffer, offset);
            value = value >> 7;
        }
    }
    return offset;
}

int write_raw_bytes(char *bytes, int bytes_size, void *_buffer, int offset) {
    int i; 
    for(i = 0; i < bytes_size; ++ i) {
        offset = write_raw_byte((char)*(bytes + i), _buffer, offset);
    }
    
    return offset;   
}

unsigned long encode_zig_zag32(signed long n) {
    // Note:  the right-shift must be arithmetic
    return (n << 1) ^ (n >> 31);
}

unsigned long long encode_zig_zag64(signed long long n) {
    // Note:  the right-shift must be arithmetic
    return (n << 1) ^ (n >> 63);
} 

signed long decode_zig_zag32(unsigned long n) {
    return (n >> 1) ^ -(n & 1);
}

signed long long decode_zig_zag64(unsigned long long n) {
    return (n >> 1) ^ -(n & 1);
}

int read_raw_byte(char *tag, void *_buffer, int offset) {
    *tag = *((char *) _buffer + offset);
    
    return ++offset;
}

/** Read a 32-bit little-endian integer from the stream. */
int read_raw_little_endian32(unsigned long *tag, void *_buffer, int offset) {
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b1 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b2 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b3 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b4 = (char) *tag;
    
    *tag = (((unsigned long)b1 & 0xff)      ) |
           (((unsigned long)b2 & 0xff) <<  8) |
           (((unsigned long)b3 & 0xff) << 16) |
           (((unsigned long)b4 & 0xff) << 24);
           
    return offset;
}

/** Read a 64-bit little-endian integer from the stream. */
int read_raw_little_endian64(unsigned long long *tag, void *_buffer, int offset) {
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b1 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b2 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b3 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b4 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b5 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b6 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b7 = (char) *tag;
    offset = read_raw_byte((char *)tag, _buffer, offset);
    char b8 = (char) *tag;
    
    *tag = (((unsigned long long)b1 & 0xff)      ) |
           (((unsigned long long)b2 & 0xff) <<  8) |
           (((unsigned long long)b3 & 0xff) << 16) |
           (((unsigned long long)b4 & 0xff) << 24) |
           (((unsigned long long)b5 & 0xff) << 32) |
           (((unsigned long long)b6 & 0xff) << 40) |
           (((unsigned long long)b7 & 0xff) << 48) |
           (((unsigned long long)b8 & 0xff) << 56);
           
    return offset;
}

int read_raw_varint32(unsigned long *tag, void *_buffer, int offset) {
    signed char result;
    
    offset = read_raw_byte((char *)&result, _buffer, offset);
    if (result >= 0) {
        *tag = result;
        return offset;
    }
    *tag = result & 0x7f;
    offset = read_raw_byte((char *)&result, _buffer, offset);
    if (result >= 0) {
        *tag |= result << 7;
    } else {
        *tag |= (result & 0x7f) << 7;
        offset = read_raw_byte((char *)&result, _buffer, offset);
        if (result >= 0) {
            *tag |= result << 14;
        } else {
            *tag |= (result & 0x7f) << 14;
            offset = read_raw_byte((char *)&result, _buffer, offset);
            if (result >= 0) {
                *tag |= ((unsigned long)result) << 21;
            } else {
                *tag |= (((unsigned long)result) & 0x7f) << 21;
                offset = read_raw_byte((char *)&result, _buffer, offset);
                *tag |= ((unsigned long)result) << 28;
                if (result < 0) {
                    // Discard upper 32 bits.
                    int i;
                    for (i = 0; i < 5; ++ i) {
                        offset = read_raw_byte((char *)&result, _buffer, offset);
                        if (result >= 0) {
                            return offset;
                        }
                    }
                    //invalid state
                }
            }
        }
    }
    return offset;
}

int read_raw_varint64(unsigned long long *tag, void *_buffer, int offset) {
    short shift = 0;
    signed char b;
    *tag = 0;
    while (shift < 64) {
        offset = read_raw_byte((char *)&b, _buffer, offset);
        *tag |= (unsigned long long)(b & 0x7F) << shift;
        if ((b & 0x80) == 0) {
            return offset;
        }
        shift += 7;
    }
    //return error code.
    return -1;
}

int can_read_raw_varint32(void *_buffer, int offset, int length) {
    signed char c;
    
    // Bound length to valid range [0..5].
    if (length < 0) length = 0; else
    if (length > 5) length = 5;
    
    while (length > 0) {
        offset = read_raw_byte((char *)&c, _buffer, offset);
        if (c >= 0) {
            return 1; // Can read (1 == true).
        }
        length--;
    }
    
    return 0; // Cannot read (0 == false).
}

int can_read_raw_varint64(void *_buffer, int offset, int length) {
    signed char c;
    
    // Bound length to valid range [0..5].
    if (length < 0) length = 0; else
    if (length > 10) length = 10;
    
    while (length > 0) {
        offset = read_raw_byte((char *)&c, _buffer, offset);
        if (c >= 0) {
            return 1; // Can read (1 == true).
        }
        length--;
    }
    
    return 0; // Cannot read (0 == false).
}

int Message_can_read_delimited_from(void *_buffer, int offset, int length) {
    unsigned long size;
    int payload_offset;

    if (length <= 0) return 0; // Cannot read from empty or invalid message.

    if (!can_read_raw_varint32(_buffer, offset, length)) {
        return 0; // Could not even read the preceding size as varint32.
    }

    payload_offset = read_raw_varint32(&size, _buffer, offset);
    length = length - (payload_offset - offset);

    return (length >= size) ? 1 : 0;
}

unsigned long Message_get_delimited_size(void *_buffer, int offset) {
    unsigned long size = 0;
    int old_offset = offset;
    
    offset = read_raw_varint32(&size, _buffer, offset);
    
    return size + offset - old_offset;
}


/*******************************************************************
 * Enum: Test.proto, line 11
 *******************************************************************/
int PhoneType_write_with_tag(enum PhoneType *_PhoneType, void *_buffer, int offset, int tag) {
    /* Write tag.*/
    offset = write_raw_varint32((tag<<3)+0, _buffer, offset);
    /* Write content.*/
    offset = write_raw_varint32(*_PhoneType, _buffer, offset);
    
    return offset;
}

/*******************************************************************
 * Message: Test.proto, line 17
 *******************************************************************/
int PhoneNumber_write(struct PhoneNumber *_PhoneNumber, void *_buffer, int offset) {
    /* Write content of each message element.*/
    offset = write_raw_varint32((1<<3)+5, _buffer, offset);
    unsigned long *number_ptr = (unsigned long *)&_PhoneNumber->_number;
    offset = write_raw_little_endian32(*number_ptr, _buffer, offset);

    offset = PhoneType_write_with_tag(&_PhoneNumber->_type, _buffer, offset, 2);
    
    return offset;
}

int PhoneNumber_write_with_tag(struct PhoneNumber *_PhoneNumber, void *_buffer, int offset, int tag) {
    /* Write tag.*/
    offset = write_raw_varint32((tag<<3)+2, _buffer, offset);
    /* Write content.*/
    offset = PhoneNumber_write_delimited_to(_PhoneNumber, _buffer, offset);
    
    return offset;
}

int PhoneNumber_write_delimited_to(struct PhoneNumber *_PhoneNumber, void *_buffer, int offset) {
    int i, shift, new_offset, size;
    
    new_offset = PhoneNumber_write(_PhoneNumber, _buffer, offset);
    size = new_offset - offset;
    shift = (size > 127) ? 2 : 1;
    for (i = new_offset - 1; i >= offset; -- i)
        *((char *)_buffer + i + shift) = *((char *)_buffer + i);
    
    write_raw_varint32((unsigned long) size, _buffer, offset);         
        
    return new_offset + shift;
}

void PhoneNumber_clear(struct PhoneNumber *_PhoneNumber) {
    _memset(_PhoneNumber, 0, sizeof(struct PhoneNumber));
}

int PhoneNumber_read(void *_buffer, struct PhoneNumber *_PhoneNumber, int offset, int limit) {
    int i = 0;
    unsigned long tag = i;
    unsigned long long value = i;

/* Reset all attributes to 0 in advance. */
    PhoneNumber_clear(_PhoneNumber);

    /* Read/interpret all attributes from buffer offset until upper limit is reached. */
    while(offset < limit) {
        offset = read_raw_varint32(&tag, _buffer, offset);
        tag = tag>>3;
        switch(tag){
            //tag of: _PhoneNumber._number 
            case 1 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                float *number = (float *)(&tag);
                _PhoneNumber->_number = *number;
                break;
            //tag of: _PhoneNumber._type 
            case 2 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _PhoneNumber->_type = tag;
                break;
        }
    }
    
    return offset;
}

int PhoneNumber_read_delimited_from(void *_buffer, struct PhoneNumber *_PhoneNumber, int offset) {
    unsigned long size;
    
    offset = read_raw_varint32(&size, _buffer, offset);
    PhoneNumber_read(_buffer, _PhoneNumber, offset, size + offset);
    
    return offset + size;
}


/*******************************************************************
 * Message: Test.proto, line 22
 *******************************************************************/
int Person_write(struct Person *_Person, void *_buffer, int offset) {
    /* Write content of each message element.*/
    offset = AddressBook_write_with_tag(&_Person->_ab, _buffer, offset, 38);
    offset = write_raw_varint32((1<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_name1_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_name1, _Person->_name1_len, _buffer, offset);

    offset = write_raw_varint32((2<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_name2_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_name2, _Person->_name2_len, _buffer, offset);

    offset = write_raw_varint32((3<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_name3_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_name3, _Person->_name3_len, _buffer, offset);

    offset = write_raw_varint32((4<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_name4_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_name4, _Person->_name4_len, _buffer, offset);

    offset = write_raw_varint32((5<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_name5_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_name5, _Person->_name5_len, _buffer, offset);

    offset = write_raw_varint32((6<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_name6_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_name6, _Person->_name6_len, _buffer, offset);

    offset = write_raw_varint32((36<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_Person->_bname_len, _buffer, offset);
    offset = write_raw_bytes(_Person->_bname, _Person->_bname_len, _buffer, offset);

    offset = write_raw_varint32((7<<3)+0, _buffer, offset);
    if (_Person->_id >= 0)
        offset = write_raw_varint32(_Person->_id, _buffer, offset);
    else
        offset = write_raw_varint64(_Person->_id, _buffer, offset);

    offset = write_raw_varint32((16<<3)+0, _buffer, offset);
    offset = write_raw_varint64(_Person->_id64, _buffer, offset);

    offset = write_raw_varint32((20<<3)+0, _buffer, offset);
    offset = write_raw_varint32(encode_zig_zag32(_Person->_sid), _buffer, offset);

    offset = write_raw_varint32((22<<3)+0, _buffer, offset);
    offset = write_raw_varint64(encode_zig_zag64(_Person->_sid64), _buffer, offset);

    offset = write_raw_varint32((24<<3)+0, _buffer, offset);
    offset = write_raw_varint32(_Person->_uid, _buffer, offset);

    offset = write_raw_varint32((26<<3)+0, _buffer, offset);
    offset = write_raw_varint64(_Person->_uid64, _buffer, offset);

    offset = write_raw_varint32((28<<3)+5, _buffer, offset);
    offset = write_raw_little_endian32(_Person->_fid32, _buffer, offset);

    offset = write_raw_varint32((30<<3)+1, _buffer, offset);
    offset = write_raw_little_endian64(_Person->_fid64, _buffer, offset);

    offset = write_raw_varint32((32<<3)+5, _buffer, offset);
    offset = write_raw_little_endian32(_Person->_sfid32, _buffer, offset);

    offset = write_raw_varint32((34<<3)+1, _buffer, offset);
    offset = write_raw_little_endian64(_Person->_sfid64, _buffer, offset);

    offset = write_raw_varint32((8<<3)+5, _buffer, offset);
    unsigned long *iq_ptr = (unsigned long *)&_Person->_iq;
    offset = write_raw_little_endian32(*iq_ptr, _buffer, offset);

    offset = write_raw_varint32((18<<3)+1, _buffer, offset);
    unsigned long long *iqd_ptr = (unsigned long long*)&_Person->_iqd;
    offset = write_raw_little_endian64(*iqd_ptr, _buffer, offset);

    offset = write_raw_varint32((9<<3)+0, _buffer, offset);
    offset = write_raw_byte(_Person->_email, _buffer, offset);

    offset = PhoneType_write_with_tag(&_Person->_phone, _buffer, offset, 10);
    int strAttr_cnt;
    for (strAttr_cnt = 0; strAttr_cnt < _Person->_strAttr_repeated_len; ++ strAttr_cnt) {
        offset = write_raw_varint32((11<<3)+2, _buffer, offset);
        offset = write_raw_varint32(_Person->_strAttr_len[strAttr_cnt], _buffer, offset);
        offset = write_raw_bytes(_Person->_strAttr[strAttr_cnt], _Person->_strAttr_len[strAttr_cnt], _buffer, offset);
    }

    int bAttr_cnt;
    for (bAttr_cnt = 0; bAttr_cnt < _Person->_bAttr_repeated_len; ++ bAttr_cnt) {
        offset = write_raw_varint32((37<<3)+2, _buffer, offset);
        offset = write_raw_varint32(_Person->_bAttr_len[bAttr_cnt], _buffer, offset);
        offset = write_raw_bytes(_Person->_bAttr[bAttr_cnt], _Person->_bAttr_len[bAttr_cnt], _buffer, offset);
    }

    int intAttr_cnt;
    for (intAttr_cnt = 0; intAttr_cnt < _Person->_intAttr_repeated_len; ++ intAttr_cnt) {
        offset = write_raw_varint32((12<<3)+0, _buffer, offset);
        if (_Person->_intAttr[intAttr_cnt] >= 0)
            offset = write_raw_varint32(_Person->_intAttr[intAttr_cnt], _buffer, offset);
        else
            offset = write_raw_varint64(_Person->_intAttr[intAttr_cnt], _buffer, offset);
    }

    int int64Attr_cnt;
    for (int64Attr_cnt = 0; int64Attr_cnt < _Person->_int64Attr_repeated_len; ++ int64Attr_cnt) {
        offset = write_raw_varint32((17<<3)+0, _buffer, offset);
        offset = write_raw_varint64(_Person->_int64Attr[int64Attr_cnt], _buffer, offset);
    }

    int sintAttr_cnt;
    for (sintAttr_cnt = 0; sintAttr_cnt < _Person->_sintAttr_repeated_len; ++ sintAttr_cnt) {
        offset = write_raw_varint32((21<<3)+0, _buffer, offset);
        offset = write_raw_varint32(encode_zig_zag32(_Person->_sintAttr[sintAttr_cnt]), _buffer, offset);
    }

    int sintAttr64_cnt;
    for (sintAttr64_cnt = 0; sintAttr64_cnt < _Person->_sintAttr64_repeated_len; ++ sintAttr64_cnt) {
        offset = write_raw_varint32((23<<3)+0, _buffer, offset);
        offset = write_raw_varint64(encode_zig_zag64(_Person->_sintAttr64[sintAttr64_cnt]), _buffer, offset);
    }

    int uintAttr_cnt;
    for (uintAttr_cnt = 0; uintAttr_cnt < _Person->_uintAttr_repeated_len; ++ uintAttr_cnt) {
        offset = write_raw_varint32((25<<3)+0, _buffer, offset);
        offset = write_raw_varint32(_Person->_uintAttr[uintAttr_cnt], _buffer, offset);
    }

    int uintAttr64_cnt;
    for (uintAttr64_cnt = 0; uintAttr64_cnt < _Person->_uintAttr64_repeated_len; ++ uintAttr64_cnt) {
        offset = write_raw_varint32((27<<3)+0, _buffer, offset);
        offset = write_raw_varint64(_Person->_uintAttr64[uintAttr64_cnt], _buffer, offset);
    }

    int fintAttr_cnt;
    for (fintAttr_cnt = 0; fintAttr_cnt < _Person->_fintAttr_repeated_len; ++ fintAttr_cnt) {
        offset = write_raw_varint32((29<<3)+5, _buffer, offset);
        offset = write_raw_little_endian32(_Person->_fintAttr[fintAttr_cnt], _buffer, offset);
    }

    int fintAttr64_cnt;
    for (fintAttr64_cnt = 0; fintAttr64_cnt < _Person->_fintAttr64_repeated_len; ++ fintAttr64_cnt) {
        offset = write_raw_varint32((31<<3)+1, _buffer, offset);
        offset = write_raw_little_endian64(_Person->_fintAttr64[fintAttr64_cnt], _buffer, offset);
    }

    int sfintAttr_cnt;
    for (sfintAttr_cnt = 0; sfintAttr_cnt < _Person->_sfintAttr_repeated_len; ++ sfintAttr_cnt) {
        offset = write_raw_varint32((33<<3)+5, _buffer, offset);
        offset = write_raw_little_endian32(_Person->_sfintAttr[sfintAttr_cnt], _buffer, offset);
    }

    int sfintAttr64_cnt;
    for (sfintAttr64_cnt = 0; sfintAttr64_cnt < _Person->_sfintAttr64_repeated_len; ++ sfintAttr64_cnt) {
        offset = write_raw_varint32((35<<3)+1, _buffer, offset);
        offset = write_raw_little_endian64(_Person->_sfintAttr64[sfintAttr64_cnt], _buffer, offset);
    }

    int boolAttr_cnt;
    for (boolAttr_cnt = 0; boolAttr_cnt < _Person->_boolAttr_repeated_len; ++ boolAttr_cnt) {
        offset = write_raw_varint32((13<<3)+0, _buffer, offset);
        offset = write_raw_byte(_Person->_boolAttr[boolAttr_cnt], _buffer, offset);
    }

    int floatAttr_cnt;
    for (floatAttr_cnt = 0; floatAttr_cnt < _Person->_floatAttr_repeated_len; ++ floatAttr_cnt) {
        offset = write_raw_varint32((14<<3)+5, _buffer, offset);
        unsigned long *floatAttr_ptr = (unsigned long *)&_Person->_floatAttr[floatAttr_cnt];
        offset = write_raw_little_endian32(*floatAttr_ptr, _buffer, offset);
    }

    int doubleAttr_cnt;
    for (doubleAttr_cnt = 0; doubleAttr_cnt < _Person->_doubleAttr_repeated_len; ++ doubleAttr_cnt) {
        offset = write_raw_varint32((19<<3)+1, _buffer, offset);
        unsigned long long *doubleAttr_ptr = (unsigned long long*)&_Person->_doubleAttr[doubleAttr_cnt];
        offset = write_raw_little_endian64(*doubleAttr_ptr, _buffer, offset);
    }

    int enumAttr_cnt;
    for (enumAttr_cnt = 0; enumAttr_cnt < _Person->_enumAttr_repeated_len; ++ enumAttr_cnt) {
        offset = PhoneType_write_with_tag(&_Person->_enumAttr[enumAttr_cnt], _buffer, offset, 15);
    }
    
    return offset;
}

int Person_write_with_tag(struct Person *_Person, void *_buffer, int offset, int tag) {
    /* Write tag.*/
    offset = write_raw_varint32((tag<<3)+2, _buffer, offset);
    /* Write content.*/
    offset = Person_write_delimited_to(_Person, _buffer, offset);
    
    return offset;
}

int Person_write_delimited_to(struct Person *_Person, void *_buffer, int offset) {
    int i, shift, new_offset, size;
    
    new_offset = Person_write(_Person, _buffer, offset);
    size = new_offset - offset;
    shift = (size > 127) ? 2 : 1;
    for (i = new_offset - 1; i >= offset; -- i)
        *((char *)_buffer + i + shift) = *((char *)_buffer + i);
    
    write_raw_varint32((unsigned long) size, _buffer, offset);         
        
    return new_offset + shift;
}

void Person_clear(struct Person *_Person) {
    _memset(_Person, 0, sizeof(struct Person));
}

int Person_read(void *_buffer, struct Person *_Person, int offset, int limit) {
    int i = 0;
    unsigned long tag = i;
    unsigned long long value = i;

/* Reset all attributes to 0 in advance. */
    Person_clear(_Person);

    /* Read/interpret all attributes from buffer offset until upper limit is reached. */
    while(offset < limit) {
        offset = read_raw_varint32(&tag, _buffer, offset);
        tag = tag>>3;
        switch(tag){
            //tag of: _Person._ab 
            case 38 :
                offset = AddressBook_read_delimited_from(_buffer, &_Person->_ab, offset);
                break;
            //tag of: _Person._name1 
            case 1 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_name1_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_name1 + i), _buffer, offset);
                break;
            //tag of: _Person._name2 
            case 2 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_name2_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_name2 + i), _buffer, offset);
                break;
            //tag of: _Person._name3 
            case 3 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_name3_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_name3 + i), _buffer, offset);
                break;
            //tag of: _Person._name4 
            case 4 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_name4_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_name4 + i), _buffer, offset);
                break;
            //tag of: _Person._name5 
            case 5 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_name5_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_name5 + i), _buffer, offset);
                break;
            //tag of: _Person._name6 
            case 6 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_name6_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_name6 + i), _buffer, offset);
                break;
            //tag of: _Person._bname 
            case 36 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_bname_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_Person->_bname + i), _buffer, offset);
                break;
            //tag of: _Person._id 
            case 7 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_id = (signed long)tag;
                break;
            //tag of: _Person._id64 
            case 16 :
                offset = read_raw_varint64(&value, _buffer, offset);
                _Person->_id64 = (signed long long)value;
                break;
            //tag of: _Person._sid 
            case 20 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_sid = decode_zig_zag32(tag);
                break;
            //tag of: _Person._sid64 
            case 22 :
                offset = read_raw_varint64(&value, _buffer, offset);
                _Person->_sid64 = decode_zig_zag64(value);
                break;
            //tag of: _Person._uid 
            case 24 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_uid = tag;
                break;
            //tag of: _Person._uid64 
            case 26 :
                offset = read_raw_varint64(&value, _buffer, offset);
                _Person->_uid64 = value;
                break;
            //tag of: _Person._fid32 
            case 28 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                _Person->_fid32 = tag;
                break;
            //tag of: _Person._fid64 
            case 30 :
                offset = read_raw_little_endian64(&value, _buffer, offset);
                _Person->_fid64 = value;
                break;
            //tag of: _Person._sfid32 
            case 32 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                _Person->_sfid32 = (signed long)tag;
                break;
            //tag of: _Person._sfid64 
            case 34 :
                offset = read_raw_little_endian64(&value, _buffer, offset);
                _Person->_sfid64 = (signed long long)value;
                break;
            //tag of: _Person._iq 
            case 8 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                float *iq = (float *)(&tag);
                _Person->_iq = *iq;
                break;
            //tag of: _Person._iqd 
            case 18 :
                offset = read_raw_little_endian64(&value, _buffer, offset);
                double *iqd = (double *)(&value);
                _Person->_iqd = *iqd;
                break;
            //tag of: _Person._email 
            case 9 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_email = tag & 1;
                break;
            //tag of: _Person._phone 
            case 10 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_phone = tag;
                break;
            //tag of: _Person._strAttr 
            case 11 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                /* Set length of current string. */
                _Person->_strAttr_len[(int)_Person->_strAttr_repeated_len] = tag;
                /* Copy raw bytes of current string. */
                for(i = 0; i < tag; ++ i) {
                    offset = read_raw_byte(&_Person->_strAttr[(int)_Person->_strAttr_repeated_len][i], _buffer, offset);
                }
                /* Advance to next string. */
                _Person->_strAttr_repeated_len++;    
                break;
            //tag of: _Person._bAttr 
            case 37 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                /* Set length of current string. */
                _Person->_bAttr_len[(int)_Person->_bAttr_repeated_len] = tag;
                /* Copy raw bytes of current string. */
                for(i = 0; i < tag; ++ i) {
                    offset = read_raw_byte(&_Person->_bAttr[(int)_Person->_bAttr_repeated_len][i], _buffer, offset);
                }
                /* Advance to next string. */
                _Person->_bAttr_repeated_len++;    
                break;
            //tag of: _Person._intAttr 
            case 12 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_intAttr[(int)_Person->_intAttr_repeated_len++] = (signed long)tag;
                break;
            //tag of: _Person._int64Attr 
            case 17 :
                offset = read_raw_varint64(&value, _buffer, offset);
                _Person->_int64Attr[(int)_Person->_int64Attr_repeated_len++] = (signed long long)value;
                break;
            //tag of: _Person._sintAttr 
            case 21 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_sintAttr[(int)_Person->_sintAttr_repeated_len++] = decode_zig_zag32(tag);
                break;
            //tag of: _Person._sintAttr64 
            case 23 :
                offset = read_raw_varint64(&value, _buffer, offset);
                _Person->_sintAttr64[(int)_Person->_sintAttr64_repeated_len++] = decode_zig_zag64(value);
                break;
            //tag of: _Person._uintAttr 
            case 25 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_uintAttr[(int)_Person->_uintAttr_repeated_len++] = tag;
                break;
            //tag of: _Person._uintAttr64 
            case 27 :
                offset = read_raw_varint64(&value, _buffer, offset);
                _Person->_uintAttr64[(int)_Person->_uintAttr64_repeated_len++] = value;
                break;
            //tag of: _Person._fintAttr 
            case 29 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                _Person->_fintAttr[(int)_Person->_fintAttr_repeated_len++] = tag;
                break;
            //tag of: _Person._fintAttr64 
            case 31 :
                offset = read_raw_little_endian64(&value, _buffer, offset);
                _Person->_fintAttr64[(int)_Person->_fintAttr64_repeated_len++] = value;
                break;
            //tag of: _Person._sfintAttr 
            case 33 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                _Person->_sfintAttr[(int)_Person->_sfintAttr_repeated_len++] = (signed long)tag;
                break;
            //tag of: _Person._sfintAttr64 
            case 35 :
                offset = read_raw_little_endian64(&value, _buffer, offset);
                _Person->_sfintAttr64[(int)_Person->_sfintAttr64_repeated_len++] = (signed long long)value;
                break;
            //tag of: _Person._boolAttr 
            case 13 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_boolAttr[(int)_Person->_boolAttr_repeated_len++] = tag & 1;
                break;
            //tag of: _Person._floatAttr 
            case 14 :
                offset = read_raw_little_endian32(&tag, _buffer, offset);
                float *floatAttr = (float *)(&tag);
                _Person->_floatAttr[(int)_Person->_floatAttr_repeated_len++] = *floatAttr;
                break;
            //tag of: _Person._doubleAttr 
            case 19 :
                offset = read_raw_little_endian64(&value, _buffer, offset);
                double *doubleAttr = (double *)(&value);
                _Person->_doubleAttr[(int)_Person->_doubleAttr_repeated_len++] = *doubleAttr;
                break;
            //tag of: _Person._enumAttr 
            case 15 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _Person->_enumAttr[(int)_Person->_enumAttr_repeated_len++] = tag;
                break;
        }
    }
    
    return offset;
}

int Person_read_delimited_from(void *_buffer, struct Person *_Person, int offset) {
    unsigned long size;
    
    offset = read_raw_varint32(&size, _buffer, offset);
    Person_read(_buffer, _Person, offset, size + offset);
    
    return offset + size;
}


/*******************************************************************
 * Message: Test.proto, line 63
 *******************************************************************/
int AddressBook_write(struct AddressBook *_AddressBook, void *_buffer, int offset) {
    /* Write content of each message element.*/
    offset = write_raw_varint32((1<<3)+2, _buffer, offset);
    offset = write_raw_varint32(_AddressBook->_address_len, _buffer, offset);
    offset = write_raw_bytes(_AddressBook->_address, _AddressBook->_address_len, _buffer, offset);

    offset = write_raw_varint32((2<<3)+0, _buffer, offset);
    if (_AddressBook->_number >= 0)
        offset = write_raw_varint32(_AddressBook->_number, _buffer, offset);
    else
        offset = write_raw_varint64(_AddressBook->_number, _buffer, offset);

    
    return offset;
}

int AddressBook_write_with_tag(struct AddressBook *_AddressBook, void *_buffer, int offset, int tag) {
    /* Write tag.*/
    offset = write_raw_varint32((tag<<3)+2, _buffer, offset);
    /* Write content.*/
    offset = AddressBook_write_delimited_to(_AddressBook, _buffer, offset);
    
    return offset;
}

int AddressBook_write_delimited_to(struct AddressBook *_AddressBook, void *_buffer, int offset) {
    int i, shift, new_offset, size;
    
    new_offset = AddressBook_write(_AddressBook, _buffer, offset);
    size = new_offset - offset;
    shift = (size > 127) ? 2 : 1;
    for (i = new_offset - 1; i >= offset; -- i)
        *((char *)_buffer + i + shift) = *((char *)_buffer + i);
    
    write_raw_varint32((unsigned long) size, _buffer, offset);         
        
    return new_offset + shift;
}

void AddressBook_clear(struct AddressBook *_AddressBook) {
    _memset(_AddressBook, 0, sizeof(struct AddressBook));
}

int AddressBook_read(void *_buffer, struct AddressBook *_AddressBook, int offset, int limit) {
    int i = 0;
    unsigned long tag = i;
    unsigned long long value = i;

/* Reset all attributes to 0 in advance. */
    AddressBook_clear(_AddressBook);

    /* Read/interpret all attributes from buffer offset until upper limit is reached. */
    while(offset < limit) {
        offset = read_raw_varint32(&tag, _buffer, offset);
        tag = tag>>3;
        switch(tag){
            //tag of: _AddressBook._address 
            case 1 :
                 /* Re-use 'tag' to store string length. */
                offset = read_raw_varint32(&tag, _buffer, offset);
                _AddressBook->_address_len = tag;
                for(i = 0; i < tag; ++ i) 
                    offset = read_raw_byte((_AddressBook->_address + i), _buffer, offset);
                break;
            //tag of: _AddressBook._number 
            case 2 :
                offset = read_raw_varint32(&tag, _buffer, offset);
                _AddressBook->_number = (signed long)tag;
                break;
        }
    }
    
    return offset;
}

int AddressBook_read_delimited_from(void *_buffer, struct AddressBook *_AddressBook, int offset) {
    unsigned long size;
    
    offset = read_raw_varint32(&size, _buffer, offset);
    AddressBook_read(_buffer, _AddressBook, offset, size + offset);
    
    return offset + size;
}


/*******************************************************************
 * Message: Test.proto, line 68
 *******************************************************************/
int Foo_write(void *_buffer, int offset) {
    /* Write content of each message element.*/
    
    return offset;
}

int Foo_write_with_tag(void *_buffer, int offset, int tag) {
    /* Write tag.*/
    offset = write_raw_varint32((tag<<3)+2, _buffer, offset);
    /* Write content.*/
    offset = Foo_write_delimited_to(_buffer, offset);
    
    return offset;
}

int Foo_write_delimited_to(void *_buffer, int offset) {
    int i, shift, new_offset, size;
    
    new_offset = Foo_write(_buffer, offset);
    size = new_offset - offset;
    shift = (size > 127) ? 2 : 1;
    for (i = new_offset - 1; i >= offset; -- i)
        *((char *)_buffer + i + shift) = *((char *)_buffer + i);
    
    write_raw_varint32((unsigned long) size, _buffer, offset);         
        
    return new_offset + shift;
}


int Foo_read(void *_buffer, int offset, int limit) {
    int i = 0;
    unsigned long tag = i;
    unsigned long long value = i;


    /* Read/interpret all attributes from buffer offset until upper limit is reached. */
    while(offset < limit) {
        offset = read_raw_varint32(&tag, _buffer, offset);
        tag = tag>>3;
        switch(tag){
        }
    }
    
    return offset;
}

int Foo_read_delimited_from(void *_buffer, int offset) {
    unsigned long size;
    
    offset = read_raw_varint32(&size, _buffer, offset);
    Foo_read(_buffer, offset, size + offset);
    
    return offset + size;
}
