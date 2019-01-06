/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render

object EnumType {

  sealed trait Type {
    def sizeInBytes: Int

    def typ: String
  }

  case object UInt8 extends Type {
    def sizeInBytes = 1

    def typ = "uint8_t"
  }

  case object UInt16 extends Type {
    def sizeInBytes = 2

    def typ = "uint16_t"
  }

  case object UInt32 extends Type {
    def sizeInBytes = 4

    def typ = "uint32_t"
  }

}

