/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.ssp21

import com.automatak.render._

object PublicKeyType extends EnumModel {

  override def name: String = "PublicKeyType"

  override def underscoredName: String = "public_key_type"

  override def comments: List[String] = List("Defines the type of the public key embedded in a certificate body")

  override def nonDefaultValues: List[EnumValue] = List(
    EnumValue("Ed25519", 0, "The key is an Ed25519 DSA public key"),
    EnumValue("X25519", 1, "The key is an x25519 DH key")
  )

  override def defaultValue: Option[EnumValue] = Some(EnumValue.undefined(255))

}



