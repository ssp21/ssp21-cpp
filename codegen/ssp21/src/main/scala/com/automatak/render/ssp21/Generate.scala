/**
 * License TBD
 */
package com.automatak.render.ssp21

import java.nio.file.{Files, Paths}

import com.automatak.render.ssp21.messages.generators.MessageGenerator

object Generate {

  val basePath = "../cpp/libs";

  val ssp21GenHeaderPath = Paths.get(basePath, "/include/ssp21/gen")
  val ssp21GenImplPath = Paths.get(basePath, "/src/ssp21/gen")

  val ssp21MsgHeaderPath = Paths.get(basePath, "/include/ssp21/msg")
  val ssp21MsgImplPath = Paths.get(basePath, "/src/ssp21/msg")

  val paths = List(ssp21GenHeaderPath, ssp21GenImplPath, ssp21MsgHeaderPath, ssp21MsgImplPath)

  def main(args: Array[String]): Unit = {

    paths.foreach(p => Files.createDirectories(p))


    AllEnums.files().foreach(_.write(ssp21GenHeaderPath, ssp21GenImplPath))


    AllMessages().map(m => MessageGenerator(m)).foreach {
      _.write(ssp21MsgHeaderPath, ssp21MsgImplPath)
    }

  }

}
