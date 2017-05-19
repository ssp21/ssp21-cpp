/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.test

import com.automatak.render._

object CryptoAction extends EnumModel {

  override def name: String = "CryptoAction"

  override def underscoredName: String = "crypto_actions"

  override def comments: List[String] = List("Enumerates possible cryptographic actions")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = None

  private val codes = List(
    EnumValue("secure_equals", 0, ""),
    EnumValue("hash_sha256", 1, ""),
    EnumValue("hmac_sha256", 2, ""),
    EnumValue("hkdf_sha256", 3, ""),
    EnumValue("gen_keypair_x25519", 4, ""),
    EnumValue("dh_x25519", 5, "")
  )

}



