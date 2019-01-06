/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21

import java.nio.file.{Files, Paths}

import org.apache.commons.io.FileUtils

object Main {

  private val basePath = "../cpp/libs/ssp21"
  private val ssp21GenHeaderPath = Paths.get(basePath, "/include/ssp21/crypto/gen")
  private val ssp21GenImplPath = Paths.get(basePath, "/src/crypto/gen")
  private val testPath = Paths.get("../cpp/tests/ssp21/gen")

  private val paths = List(ssp21GenHeaderPath, ssp21GenImplPath, testPath)

  def main(args: Array[String]): Unit = {

    paths.foreach(path => FileUtils.deleteDirectory(path.toFile))

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