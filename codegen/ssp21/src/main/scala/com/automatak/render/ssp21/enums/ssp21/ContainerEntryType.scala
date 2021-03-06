/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object ContainerEntryType extends EnumModel {

  override def name: String = "ContainerEntryType"

  override def underscoredName: String = "container_entry_type"

  override def comments: List[String] = List("enumerates the possible entry types in a container file")

  override def nonDefaultValues: List[EnumValue] = List(
    EnumValue("shared_secret", 0, "256-bit shared secret"),
    EnumValue("certificate_chain", 1, "industrial certificate chain"),
    EnumValue("x25519_public_key", 2, "X25519 private key"),
    EnumValue("x25519_private_key", 3, "X25519 private key"),
    EnumValue("ed25519_public_key", 4, "Ed25519 private key"),
    EnumValue("ed25519_private_key", 5, "Ed25519 private key")

  )

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

}



