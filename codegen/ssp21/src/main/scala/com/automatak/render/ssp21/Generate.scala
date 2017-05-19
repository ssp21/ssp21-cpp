/**
  * License TBD
  */
package com.automatak.render.ssp21

import java.nio.file.{Files, Paths}

object Generate {

  val basePath = "../cpp/libs";

  val ssp21GenHeaderPath = Paths.get(basePath, "/include/ssp21/crypto/gen")
  val ssp21GenImplPath = Paths.get(basePath, "/src/ssp21/crypto/gen")
  val testPath = Paths.get("../cpp/tests/ssp21/gen")

  val paths = List(ssp21GenHeaderPath, ssp21GenImplPath, testPath)

  def main(args: Array[String]): Unit = {

    paths.foreach(p => Files.createDirectories(p))

    GeneratedFiles.api.foreach { f =>
      f.write(ssp21GenHeaderPath, ssp21GenImplPath)
    }

    GeneratedFiles.impl.foreach { f =>
      f.write(ssp21GenImplPath, ssp21GenImplPath)
    }

    TestFiles.list.foreach { f =>
      f.write(testPath, testPath)
    }

  }

}