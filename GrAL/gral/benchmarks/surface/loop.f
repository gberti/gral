      SUBROUTINE LOOPF(nloop,ntri, til, nv, geom, surf)
      IMPLICIT NONE
      INTEGER nloop, ntri, nv
      INTEGER til(1:3,1:ntri)
      DOUBLE PRECISION  geom(1:2,1:nv)
      DOUBLE PRECISION  surf(1:ntri)
      DOUBLE PRECISION  dx, dy
      INTEGER c, vc, vc1, N
      DO 30 N = 1, nloop
         DO 20 c = 1,ntri
            surf(c) = 0.0
            DO 10 vc = 1, 3
               vc1 = vc +1
               IF (vc1 .eq. 4) THEN
                  vc1 = 1
               ENDIF
               dx = (geom(1,til(vc ,c)) - geom(1,til(vc1,c)))
               dy = (geom(2,til(vc1,c)) - geom(2,til(vc ,c)))
               surf(c) = sqrt(dx*dx+dy*dy)
 10         CONTINUE
 20      CONTINUE 
 30   CONTINUE
      RETURN
      END
