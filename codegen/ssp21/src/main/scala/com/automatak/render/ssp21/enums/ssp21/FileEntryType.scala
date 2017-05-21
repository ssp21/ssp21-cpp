/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object FileEntryType extends EnumModel {

  override def name: String = "FileEntryType"

  override def underscoredName: String = "file_entry_type"

  override def comments: List[String] = List("enumerates the possible entry types in a certificate file")

  override def nonDefaultValues: List[EnumValue] = List(
    EnumValue("certificate", 0, "Industrial certificate format (ICF)"),
    EnumValue("x25519_public_key", 1, "X25519 private key"),
    EnumValue("x25519_private_key", 2, "X25519 private key"),
    EnumValue("ed25519_public_key", 3, "Ed25519 private key"),
    EnumValue("ed25519_private_key", 4, "Ed25519 private key")

  )

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

}



