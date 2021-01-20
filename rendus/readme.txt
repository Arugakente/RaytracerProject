GOSSET Guillaume
LASOTA Quentin
Week number of assignment: 3

bin: contains the binary and the scenes, plus a batch script that produces each result
      (note : some of the yaml scene are not computed since it would take too much time to compute everything)
img: contains the result images and error images
src: contains every .cpp and .h file (+ the Makefile) at the end of the assignment, 
      plus the solution (.sln) file

bonuses :
 + fixed the specular effect with refraction
 - xStretch (zoom only in x direction)
 - deep of field (DoF) with blurring effect (no "rings")
 - soft shadows (with lightSamples)
 - velocity (with exposureTime and exposureSamples)
 - multithreading with OpenMP
 - alpha (material transparency) computation

 - moved our zbuffer to zbufferAuto (and implemented zbuffer with far and near parameters)
 - managed YAML file reading so it is retro-compatible with old files

!!! If you want to compute the fullset (you should check the YAML config file), acknowledge that it will take some hours !!!