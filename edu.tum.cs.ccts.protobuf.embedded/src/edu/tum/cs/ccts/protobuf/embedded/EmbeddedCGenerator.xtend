package edu.tum.cs.ccts.protobuf.embedded

import java.io.File
import java.io.FileWriter
import org.antlr.runtime.tree.CommonTree

import static extension edu.tum.cs.ccts.protobuf.embedded.TreeUtils.*

class EmbeddedCGenerator {
	
	val typeMap = newHashMap(   "double"->"double",
  							    "float"->"float",
  								"int32"->"signed long",
  								"int64"->"signed long long",
  								"uint32"->"unsigned long",
  								"uint64"->"unsigned long long",
  								"sint32"->"signed long",
  								"sint64"->"signed long long",
  								"fixed32"->"unsigned long",
  								"fixed64"->"unsigned long long",
  								"sfixed32"->"signed long",
  								"sfixed64"->"signed long long",
  								"bool"->"char",
  								"string"->"char",
  								"bytes"->"char"
  	);
  	  	
  	val wireTypeMap = newHashMap(	"int32"->"0", 
  									"int64"->"0",
  									"sint32"->"0",
  									"sint64"->"0",
  									"uint32"->"0",
                     				"uint64"->"0",
                     				"string"->"2",
                     				"bool"->"0",
                     				"float"->"5",
                     				"double"->"1",
                        			"fixed32"->"5",
                        			"fixed64"->"1",
                        			"sfixed32"->"5",
                        			"sfixed64"->"1",
                        			"bytes"->"2"
	);
  	
  	val enumSet = newHashSet()
  
    val arrayTypes = newHashMap(  "string"->"[MAX_STRING_LENGTH]", 
    							  "bytes"->"[MAX_BYTES_LENGTH]"
    ); 
    
    val readValues = newHashMap(  "int32"->"(signed long)tag", 
    							  "int64"->"(signed long long)value", 
    							  "sint32"->"decode_zig_zag32(tag)",
    							  "sint64"->"decode_zig_zag64(value)",
    							  "uint32"->"tag", 
    							  "uint64"->"value", 
    							  "fixed32"->"tag", 
    							  "fixed64"->"value", 
    							  "sfixed32"->"(signed long)tag", 
    							  "sfixed64"->"(signed long long)value",
    							  "bool"->"tag & 1"
    ); 
	
	def doGenerate(File outputDirectory, String name, CommonTree tree) {
		generateFile(new File(outputDirectory, name + ".h"), tree.compileHeader(name))
		generateFile(new File(outputDirectory, name + ".c"), tree.compileImplementation(name))
	}

	
	def compileHeader(CommonTree tree, String name) '''
		/******************************************************************* 
		 * Header file generated by Protoc for Embedded C.                 *
		 * Version «Protoc::VERSION»                                      *
		 *                                                                 *
		 * Copyright (c) 2009-2013                                         *
		 * Technische Universitaet Muenchen                                *
		 * http://www4.in.tum.de/                                          *
		 *                                                                 *
		 * Source : «name.toFirstUpper».proto
		 *                                                                 *
		 * Do not edit.                                                    *
		 *******************************************************************/

		#ifndef _«name.toFirstUpper»_H
		#define _«name.toFirstUpper»_H
		
		#ifdef __cplusplus
		  extern "C" {
		#endif
		
		«FOR d : tree.childTrees.filter[it.type == ProtoParser::ANNOTATION]»
			«d.getDefine»
		«ENDFOR»
		
		/*******************************************************************
		 * General functions
		 *******************************************************************/
		
		/*
		 * returns the size of a length delimited message which also 
		 * contains the first bytes for the length encoding.
		 */
		unsigned long Message_get_delimited_size(void *_buffer, int offset);
		
		/*
		 * Tests whether a message can be completely read from the given buffer at
		 * the offset. The bytes [offset..offset+length-1] are interpreted.
		 *
		 * Returns 1 (true) if buffer[offset..offset+length-1] contains a complete
		 * message or 0 (false) otherwise.
		 */
		int Message_can_read_delimited_from(void *_buffer, int offset, unsigned int length);
		
		
		«FOR e : tree.childTrees.filter[it.type == ProtoParser::ENUM]»
		 /*******************************************************************
		 * Enumeration: «name.toFirstUpper».proto
		 *******************************************************************/
		 «e.getEnumHeader»
		 
		 
		«ENDFOR»	
		
		«FOR m : tree.sortedChildTreesWithType(ProtoParser::MESSAGE)»
		/*******************************************************************
		 * Message: «name.toFirstUpper».proto
		 *******************************************************************/
		«m.getMessageHeader»
		
		/*
		 * Serialize a Person-message into the given buffer at offset and return
		 * new offset for optional next message.
		 */
		int «m.getMessageName»_write_delimited_to(struct «m.getMessageName» *_«m.getMessageName», void *_buffer, int offset);
		
		/*
		 * Serialize a Person-message together with its tag into the given buffer 
		 * at offset and return new offset for optional next message.
		 * Is useful if a Person-message is embedded in another message.
		 */
		int «m.getMessageName»_write_with_tag(struct «m.getMessageName» *_«m.getMessageName», void *_buffer, int offset, int tag);
		
		/*
		 * Deserialize a Person-message from the given buffer at offset and return
		 * new offset for optional next message.
		 *
		 * Note: All fields in _Person will be reset to 0 before _buffer is interpreted.
		 */
		int «m.getMessageName»_read_delimited_from(void *_buffer, struct «m.getMessageName» *_«m.getMessageName», int offset);
		
		
		«ENDFOR»
		
		#ifdef __cplusplus
		  }
		#endif
		
		#endif

	'''
	
	
	def getMessageName(CommonTree m) { 
		val name = m.children.get(0) as CommonTree
		
		name.text
	}

	
	def getMessageHeader(CommonTree m) { 
		var messageValue = new StringBuilder()
		val name = m.children.get(0) as CommonTree
		
		messageValue.append("/* Structure that holds a deserialized " + name.text + "-message. */\n");
		messageValue.append("struct ");
		
		messageValue.append(name.text + " {\n");
		
		val indent = "    "
		var i = 1;
		while(i < m.children.size()) {
			val modifier = (m.children.get(i) as CommonTree).children.get(0) as CommonTree
			val type = (m.children.get(i) as CommonTree).children.get(1) as CommonTree
			val attrName = (m.children.get(i) as CommonTree).children.get(2) as CommonTree
			
			var cType = typeMap.get(type.text);
			
			if (cType == null) {
				if (enumSet.contains(type.text)) {
					cType = "enum " + type.text;
				}else {
					cType = "struct " + type.text;	
				}
			}
				
			var repValue = ""
			if (modifier.text.equals("repeated")) {
				repValue = "[MAX_REPEATED_LENGTH]"
			}
			
			var	typeArrayValue = arrayTypes.get(type.text)
			if (typeArrayValue == null) {
				typeArrayValue = ""
			}
			
			if (!repValue.equals("")) {
				messageValue.append(indent + "int" + " _" + attrName.text 
									+ "_repeated_len" + ";\n"
				);
			}
			if (!typeArrayValue.equals("")) {
				messageValue.append(indent + "int" + " _" + attrName.text + "_len" 
									+ repValue + ";\n"
				);
			}
			
			messageValue.append(indent + cType + " _" + attrName.text 
									+ repValue + typeArrayValue + ";\n")
			
			i = i + 1
		}  
		messageValue.append("};")
		
		messageValue.toString
	}

	
	def getEnumHeader(CommonTree e) { 
		var enumValue = new StringBuilder()
		enumValue.append("enum ");
		
		val name = e.children.get(0) as CommonTree
		
		enumValue.append(name + " {\n");
		enumSet.add(name.text);
		
		val indent = "    "
		var i = 1;
		while(i < e.children.size()-1) {
			val value = (e.children.get(i) as CommonTree).children.get(0) as CommonTree
			val id = (e.children.get(i) as CommonTree).children.get(1) as CommonTree
			
			enumValue.append(indent + "_" + value.text + " = " + id.text + ",\n")
			
			i = i + 1
		}  
		val value = (e.children.get(i) as CommonTree).children.get(0) as CommonTree
		val id = (e.children.get(i) as CommonTree).children.get(1) as CommonTree
			
		enumValue.append(indent + "_" + value.text + " = " + id.text + "\n")
		enumValue.append("};")
		
		enumValue.toString
	}
	
	
	def getDefine(CommonTree d) {
		var define = "#define "
		
		val literal = d.children.get(0) as CommonTree
		val number = d.children.get(1) as CommonTree
		define = define + literal.text.toUpperCase + " "
		define = define + number.text
	}



	def CharSequence compileImplementation(CommonTree tree, String name) '''
		/******************************************************************* 
		 * C file generated by Protoc for Embedded C.                      *
		 * Version «Protoc::VERSION»                                      *
		 *                                                                 *
		 * Copyright (c) 2009-2013                                         *
		 * Technische Universitaet Muenchen                                *
		 * http://www4.in.tum.de/                                          *
		 *                                                                 *
		 * Source : «name».proto
		 *                                                                 *
		 * Do not edit.                                                    *
		 *******************************************************************/
		 
		#include "«name.toFirstUpper».h"
		
		int _memcmp(const void *p1, const void *p2, unsigned int size) {
		    unsigned int i;
		    for(i = 0; i < size; ++ i) {
		        if(*((char*)p1 + i) > *((char*)p2 + i))
		            return 1;
		        if(*((char*)p1 + i) < *((char*)p2 + i))
		            return -1;
		    }
		    return 0;
		} 
		 
		void _memset(void *msg_ptr, char init_val, unsigned int size) {
		    unsigned int i;
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
		    /* Note:  the right-shift must be arithmetic. */
		    return (n << 1) ^ (n >> 31);
		}
		
		unsigned long long encode_zig_zag64(signed long long n) {
		    /* Note:  the right-shift must be arithmetic. */
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
		                    /* Discard upper 32 bits. */
		                    int i;
		                    for (i = 0; i < 5; ++ i) {
		                        offset = read_raw_byte((char *)&result, _buffer, offset);
		                        if (result >= 0) {
		                            return offset;
		                        }
		                    }
		                    /* Invalid state. */
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
		    /* return error code. */
		    return -1;
		}
		
		int can_read_raw_varint32(void *_buffer, int offset, int length) {
		    signed char c;
		    
		    /* Bound length to valid range [0..5]. */
		    if (length < 0) length = 0; else
		    if (length > 5) length = 5;
		    
		    while (length > 0) {
		        offset = read_raw_byte((char *)&c, _buffer, offset);
		        if (c >= 0) {
		            return 1; /* Can read (1 == true). */
		        }
		        length--;
		    }
		    
		    return 0; /* Cannot read (0 == false). */
		}
		
		int can_read_raw_varint64(void *_buffer, int offset, int length) {
		    signed char c;
		    
		    /* Bound length to valid range [0..5]. */
		    if (length < 0) length = 0; else
		    if (length > 10) length = 10;
		    
		    while (length > 0) {
		        offset = read_raw_byte((char *)&c, _buffer, offset);
		        if (c >= 0) {
		            return 1; /* Can read (1 == true). */
		        }
		        length--;
		    }
		    
		    return 0; /* Cannot read (0 == false). */
		}
		
		int Message_can_read_delimited_from(void *_buffer, int offset, unsigned int length) {
		    unsigned long size;
		    int payload_offset;
		
		    if (length <= 0) return 0; /* Cannot read from empty or invalid message. */
		
		    if (!can_read_raw_varint32(_buffer, offset, length)) {
		        return 0; /* Could not even read the preceding size as varint32. */
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

		«FOR e : tree.childTrees.filter[it.type == ProtoParser::ENUM]»
		/*******************************************************************
		* Enumeration: «name.toFirstUpper».proto
		*******************************************************************/
		«e.getEnumImplementation»
		
		«ENDFOR»
		
		«FOR m : tree.childTrees.filter[it.type == ProtoParser::MESSAGE]»
		/*******************************************************************
		 * Message: «name.toFirstUpper».proto
		 *******************************************************************/
		
		void «m.messageName»_clear(struct «m.messageName» *_«m.messageName») {
			_memset(_«m.messageName», 0, sizeof(struct «m.messageName»));
		}
		
		void «m.messageName»_init_optional_attributes(struct «m.messageName» *_«m.messageName») {
			«m.initOptionalAttributes»
		}
		
		int «m.messageName»_is_default_message(struct «m.messageName» *_«m.messageName») {
		    return «m.isDefaultMessage»
		}
		
		int «m.messageName»_write(struct «m.messageName» *_«m.messageName», void *_buffer, int offset) {
			«m.writeMessage»
		}
		
		int «m.getMessageName»_write_with_tag(struct «m.getMessageName» *_«m.getMessageName», void *_buffer, int offset, int tag) {
			«m.writeWithTag»
		}
		
		int «m.getMessageName»_write_delimited_to(struct «m.getMessageName» *_«m.getMessageName», void *_buffer, int offset) {
			«m.writeDelimitedTo»
		}
		
		int «m.getMessageName»_read(void *_buffer, struct «m.getMessageName» *_«m.getMessageName», int offset, int limit) {
			«m.readMessage»
		}
		
		int «m.getMessageName»_read_delimited_from(void *_buffer, struct «m.getMessageName» *_«m.getMessageName», int offset) {
			// TODO: generate code
		    return 1;
		}
		«ENDFOR»	
	'''
	
	
	def readMessage(CommonTree m) { 
		val assignment = new StringBuilder;
		
		val curMessage = m.getChild(0) as CommonTree
				
		assignment.append('''
			unsigned int i = 0;
			unsigned long long value = i;
			unsigned long tag = value;
			
			/* Reset all attributes to 0 in advance. */
			«curMessage.text»_clear(_«curMessage.text»);
			/* Assign the optional attributes. */
			«curMessage.text»_init_optional_attributes(_«curMessage.text»);
			
			/* Read/interpret all attributes from buffer offset until upper limit is reached. */
			while(offset < limit) {
			    offset = read_raw_varint32(&tag, _buffer, offset);
				tag = tag>>3;
			    switch(tag){
			        «FOR attr : m.childTrees.tail»
			        	«attr.caseElementTag(curMessage)»
			        «ENDFOR»
			    }
			}
			
			return offset;
			''')
	}
	
	
	def caseElementTag(CommonTree attr, CommonTree parent) { 
		val tag = (attr as CommonTree).children.get(3) as CommonTree
		val attrName = (attr as CommonTree).children.get(2) as CommonTree
		val type = (attr as CommonTree).children.get(1) as CommonTree
		val modifier = (attr as CommonTree).children.get(0) as CommonTree
		
		'''
		/* tag of: _«parent.text»._«attrName.text» */
		case «tag» :
			«IF type.text.equals("int32") || type.text.equals("sint32")
					|| type.text.equals("uint32") || type.text.equals("bool")
					|| enumSet.contains(type.text)»
			offset = read_raw_varint32(&tag, _buffer, offset);
			«ELSEIF type.text.equals("string") || type.text.equals("bytes")»
			/* Re-use 'tag' to store string length. */
			offset = read_raw_varint32(&tag, _buffer, offset);
			«ELSEIF type.text.equals("int64") || type.text.equals("uint64")
					 || type.text.equals("sint64")»
			offset = read_raw_varint64(&value, _buffer, offset);
			«ELSEIF type.text.equals("fixed32") || type.text.equals("sfixed32")»
			offset = read_raw_little_endian32(&tag, _buffer, offset);
			«ELSEIF type.text.equals("fixed64") || type.text.equals("sfixed64")»
			offset = read_raw_little_endian64(&value, _buffer, offset);
			«ELSEIF type.text.equals("float")»
			offset = read_raw_little_endian32(&tag, _buffer, offset);
			float *«attrName.text» = (float *)(&tag);
			«ELSEIF type.text.equals("double")»
			offset = read_raw_little_endian64(&value, _buffer, offset);
			double *«attrName.text» = (double *)(&value);
			«ENDIF»
			«IF readValues.containsKey(type.text)»
			«IF modifier.text.equals("repeated")»
			_«parent.text»->_«attrName.text»[(int)_«parent.text»->_«attrName.text»_repeated_len++] = «readValues.get(type.text)»;
			«ELSE»
			_«parent.text»->_«attrName.text» = «readValues.get(type.text)»;
			«ENDIF»
			«ELSEIF type.text.equals("string") || type.text.equals("bytes")»
			«IF modifier.text.equals("repeated")»
				/* Set length of current string. */
			_«parent.text»->_«attrName.text»_len[(int)_«parent.text»->_«attrName.text»_repeated_len] = tag;
			/* Copy raw bytes of current string. */
			for(i = 0; i < tag; ++ i) {
			    offset = read_raw_byte(&_«parent.text»->_«attrName.text»[(int)_«parent.text»->_«attrName.text»_repeated_len][i], _buffer, offset);
			}
			/* Advance to next string. */
			_«parent.text»->_«attrName.text»_repeated_len++; 
			«ELSE»
			_«parent.text»->_«attrName.text»_len = tag;
			for(i = 0; i < tag; ++ i) 
			    offset = read_raw_byte((_«parent.text»->_«attrName.text» + i), _buffer, offset);
			«ENDIF»
			«ELSEIF type.text.equals("float") || type.text.equals("double")»
			«IF modifier.text.equals("repeated")»
			_«parent.text»->_«attrName.text»[(int)_«parent.text»->_«attrName.text»_repeated_len++] = *«attrName.text»;
			«ELSE»
			_«parent.text»->_«attrName.text» = *«attrName.text»;
			«ENDIF»
			«ELSEIF enumSet.contains(type.text)»
			«IF modifier.text.equals("repeated")»
			_«parent.text»->_«attrName.text»[(int)_«parent.text»->_«attrName.text»_repeated_len++] = tag;
			«ELSE»
			_«parent.text»->_«attrName.text» = tag;
			«ENDIF»
			«ELSE»
			«IF modifier.text.equals("repeated")»
			offset = «type.text»_read_delimited_from(_buffer, &_«parent.text»->_«attrName.text»[(int)_«parent.text»->_«attrName.text»_repeated_len++], offset);
			«ELSE»
			offset = «type.text»_read_delimited_from(_buffer, &_«parent.text»->_«attrName.text», offset);
			«ENDIF»
			«ENDIF»
			break;
		'''
	}
	
	
	def writeDelimitedTo(CommonTree m) { 
		val assignment = new StringBuilder;
		
		val curMessage = m.getChild(0) as CommonTree
				
		assignment.append('''
		int i, shift, new_offset, size;
		
		new_offset = «curMessage.text»_write(_«curMessage.text», _buffer, offset);
		size = new_offset - offset;
		shift = (size > 127) ? 2 : 1;
		for (i = new_offset - 1; i >= offset; -- i)
		    *((char *)_buffer + i + shift) = *((char *)_buffer + i);
		
		write_raw_varint32((unsigned long) size, _buffer, offset);         
		    
		return new_offset + shift;
		''')
	}
	
	
	def writeWithTag(CommonTree m) { 
		val assignment = new StringBuilder;
		
		val curMessage = m.getChild(0) as CommonTree
				
		assignment.append('''
		/* Write tag.*/
		offset = write_raw_varint32((tag<<3)+2, _buffer, offset);
		/* Write content.*/
		offset = «curMessage.text»_write_delimited_to(_«curMessage.text», _buffer, offset);
		
		return offset;
		''')
		
	}
	
	
	def writeMessage(CommonTree m) {
		val assignment = new StringBuilder;
		
		assignment.append('''
		/* Write content of each message element.*/
		''')
		
		val curMessage = m.getChild(0) as CommonTree
		for (CommonTree attr : m.childTrees.tail) {
			val modifier = (attr as CommonTree).children.get(0) as CommonTree
			val type = (attr as CommonTree).children.get(1) as CommonTree
			val attrName = (attr as CommonTree).children.get(2) as CommonTree
			val tag = (attr as CommonTree).children.get(3) as CommonTree
						
			if (modifier.text.equals("repeated")) {
				assignment.append('''
				int «attrName.text»_cnt;
				for («attrName.text»_cnt = 0; «attrName.text»_cnt < _«curMessage.text»->_«attrName.text»_repeated_len; ++ «attrName.text»_cnt) {
				    «IF !enumSet.contains(type.text)»
				    «type.writeTag(tag)»
				    «ENDIF»
				    «attr.writeTypeNoTag(curMessage, "["+attrName.text+"_cnt]")»
				}
				
				''')
				
			} else if (modifier.text.equals("optional")) {
				assignment.append('''
				/* Write the optional attribute only if it is different than the default value. */
				«IF typeMap.containsKey(type.text) || enumSet.contains(type.text)»
				if(_«curMessage.text»->_«attrName.text» != 0) {
				«ELSE»
				if(!«type.text»_is_default_message(&_«curMessage.text»->_«attrName»)) {
				«ENDIF»
				    «IF !enumSet.contains(type.text) && typeMap.containsKey(type.text)»
					«type.writeTag(tag)»
					«ENDIF»
				    «attr.writeTypeNoTag(curMessage, "")»
				}
				
				''')
			} else {
				assignment.append('''
				«IF !enumSet.contains(type.text) && typeMap.containsKey(type.text)»
				«type.writeTag(tag)»
				«ENDIF»
				«attr.writeTypeNoTag(curMessage, "")»
				
				''')
			}
			
		}
		
		assignment.append("return offset;\n");
	}
	
	def writeTypeNoTag(CommonTree attr, CommonTree curMessage, String arrayPart) {
		val type = (attr as CommonTree).children.get(1) as CommonTree
		val attrName = (attr as CommonTree).children.get(2) as CommonTree
		val tag = (attr as CommonTree).children.get(3) as CommonTree
		 
		if (type.text.equals("int32")) {
			'''
			if (_«curMessage.text»->_«attrName.text + arrayPart» >= 0)
			    offset = write_raw_varint32(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);
			else
			    offset = write_raw_varint64(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);	    
			'''
		} else if (type.text.equals("int64") || type.text.equals("uint64")) {
			'''offset = write_raw_varint64(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);'''
		} else if (type.text.equals("sint32")) {
			'''offset = write_raw_varint32(encode_zig_zag32(_«curMessage.text»->_«attrName.text + arrayPart»), _buffer, offset);'''
		} else if (type.text.equals("sint64")) {
			'''offset = write_raw_varint32(encode_zig_zag64(_«curMessage.text»->_«attrName.text + arrayPart»), _buffer, offset);'''
		} else if (type.text.equals("uint32")) {
			'''offset = write_raw_varint32(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);'''
		} else if (type.text.equals("fixed32") || type.text.equals("sfixed32")) {
			'''offset = write_raw_little_endian32(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);'''
		} else if (type.text.equals("fixed64") || type.text.equals("sfixed64")) {
			'''offset = write_raw_little_endian64(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);'''
		} else if (type.text.equals("bytes") || type.text.equals("string")) {
			'''
			offset = write_raw_varint32(_«curMessage.text»->_«attrName.text»_len«arrayPart», _buffer, offset);
			offset = write_raw_bytes(_«curMessage.text»->_«attrName.text + arrayPart», _«curMessage.text»->_«attrName.text»_len«arrayPart», _buffer, offset);
			'''
		} else if (type.text.equals("bool")) {
			'''offset = write_raw_byte(_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset);'''
		} else if (type.text.equals("float")) {
			'''
			unsigned long *«attrName.text»_ptr = (unsigned long *)&_«curMessage.text»->_«attrName.text + arrayPart»;
			offset = write_raw_little_endian32(*«attrName.text»_ptr, _buffer, offset);
			'''
		} else if (type.text.equals("double")) {
			'''
			unsigned long long*«attrName.text»_ptr = (unsigned long long*)&_«curMessage.text»->_«attrName.text + arrayPart»;
			offset = write_raw_little_endian64(*«attrName.text»_ptr, _buffer, offset);
			'''
		} else if (enumSet.contains(type.text) || !typeMap.containsKey(type.text)) {
			'''offset = «type.text»_write_with_tag(&_«curMessage.text»->_«attrName.text + arrayPart», _buffer, offset, «tag.text»);'''
		}
		
	}
	
	
	def writeTag(CommonTree type, CommonTree tag) { 
		if (wireTypeMap.containsKey(type.text)) {
			'''
			offset = write_raw_varint32((«tag.text»<<3)+«wireTypeMap.get(type.text)», _buffer, offset);
			'''
		}
	}

	
	def isDefaultMessage(CommonTree m) { 
		val conditions = new StringBuilder
		
		var i = 1;
		while(i < m.children.size()) {
			val modifier = (m.children.get(i) as CommonTree).children.get(0) as CommonTree
			val type = (m.children.get(i) as CommonTree).children.get(1) as CommonTree
			val attrName = (m.children.get(i) as CommonTree).children.get(2) as CommonTree
			
			var cType = typeMap.get(type.text);
			var condition = ""
		
			if (cType == null) {
				if (enumSet.contains(type.text)) {
					// Enum.
					condition = "_" + m.messageName + "->_" + attrName.text + " == 0"
				} else {
					// Struct.
					condition = type.text + "_is_default_message(&_" + m.messageName + "->_" + attrName.text +  ")"	
				}
			} else {
				// Normal data types.
				if ( cType.equals("signed long") || cType.equals("signed long long") 
						|| cType.equals("unsigned long") || cType.equals("unsigned long long")
						|| 	cType.equals("char") ) {
					condition = "_" + m.messageName + "->_" + attrName.text + " == 0"			
				}
				
				if ( cType.equals("float") ) {
					condition = "_" + m.messageName + "->_" + attrName.text + " == 0.0f"
				}
				if ( cType.equals("double") ) {
					condition = "_" + m.messageName + "->_" + attrName.text + " == 0.0"
				}
			}
			
			if (i < m.children.size() - 1) {
				condition = condition + "\n && "
			} else {
				condition = condition + ";"
			}
			
			if (!modifier.text.equals("repeated"))	
				conditions.append(condition);
				
			i = i + 1
		}  
		
		conditions.toString();
	}
	
	
	def initOptionalAttributes(CommonTree m) {
		val assignments = new StringBuilder();
		
		var i = 1;
		while(i < m.children.size()) {
			val modifier = (m.children.get(i) as CommonTree).children.get(0) as CommonTree
			val type = (m.children.get(i) as CommonTree).children.get(1) as CommonTree
			val attrName = (m.children.get(i) as CommonTree).children.get(2) as CommonTree
			
			if (modifier.text.equals("optional")) {
				var cType = typeMap.get(type.text)
				var isArrayType = arrayTypes.containsKey(type.text)
				var assignment = ""
			
				if (cType == null) {
					if (enumSet.contains(type.text)) {
						// Enum.
						assignment = "_" + m.messageName + "->_" + attrName.text + " = 0;"
					} else {
						// Struct.
						assignment = type.text + "_init_optional_attributes(&_" + m.messageName + "->_" + attrName.text +  ");"	
					}
				} else {
					// Normal data types.
					if ( cType.equals("signed long") || cType.equals("signed long long") 
							|| cType.equals("unsigned long") || cType.equals("unsigned long long")
							|| 	cType.equals("char") ) {
						if (isArrayType)
							assignment = "_" + m.messageName + "->_" + attrName.text + "_len" + " = 0;"
						else			
							assignment = "_" + m.messageName + "->_" + attrName.text + " = 0;"
					}
					
					if ( cType.equals("float") ) {
						assignment = "_" + m.messageName + "->_" + attrName.text + " = 0.0f;"
					}
					if ( cType.equals("double") ) {
						assignment = "_" + m.messageName + "->_" + attrName.text + " = 0.0;"
					}
				
				}
				
				assignments.append(assignment + "\n\n");
			}	
			i = i + 1
		}  
		
		assignments.toString();
	}


	def getEnumImplementation(CommonTree e) '''
		«val name = e.children.get(0) as CommonTree»
		int «name.text»_write_with_tag(enum «name.text» *_«name.text», void *_buffer, int offset, int tag) {
		    /* Write tag.*/
		    offset = write_raw_varint32((tag<<3)+0, _buffer, offset);
		    /* Write content.*/
		    offset = write_raw_varint32(*_«name.text», _buffer, offset);
		    
		    return offset;
		}
	'''
	
	
	/**
	 * Creates the given file and writes the given contents to it.
	 */
	def generateFile(File file, CharSequence contents) {
		val writer = new FileWriter(file, false)
		writer.write(contents.toString)
		writer.close
	}
	 
}