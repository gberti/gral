      SUBROUTINE LOOPMOD(nloop,ntri, til, nv, geom, nx, ny)
      IMPLICIT NONE
      INTEGER nloop, ntri, nv
      INTEGER til(1:3,1:ntri)
      DOUBLE PRECISION  geom(1:2,0:nv-1)
      DOUBLE PRECISION nx, ny
      INTEGER c, vc, vc1, N
      DO 30 N = 1, nloop
         nx = 0.0
         ny = 0.0
         DO 20 c = 1,ntri
            DO 10 vc = 1, 3
               vc1 = MOD(vc,3) +1
               nx = nx + (geom(2,til(vc1,c)) - geom(2,til(vc ,c)))
               ny = ny + (geom(1,til(vc ,c)) - geom(1,til(vc1,c)))
 10         CONTINUE
 20      CONTINUE 
 30   CONTINUE
      RETURN
      END
