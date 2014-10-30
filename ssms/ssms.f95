program ssms

  implicit none
  !Sparse Matrix Storage Formats
  !store all non-zero elements of the matrix into a linear array
  ! and provide auxiliary arrays to describe the locations of the
  ! non-zero elements in the original matrix.
  !
  !Yale format! CRS
  
  type sm
     real, pointer, dimension(:) :: A      ! real value of the non-zero elem
     integer, pointer, dimension(:) :: IA  ! ith row has elem A(IA(I))to A(IA(I+1)-1)
     integer, pointer, dimension(:) :: JA  ! ith row has J index IJ(IA(I) : IA(I+1)-1)
     integer :: N                          ! dimension of the matrix  
     integer :: NNZ                        ! number of non-zero elem
     ! can be accessed by IA(N+1)-1
  end type sm

  integer, Parameter :: n = 5
  type(sm) :: A
  integer :: I
  real, dimension(n) :: x,b 

  integer :: max_it 
  real :: tol
  integer :: flag 
  !integer :: I,J
  ! test the following sparse matrix NNZ = 7 , N = 5
  !
  ! 1 x x 5 x
  ! x 2 x x x
  ! 1 x 2 4 x
  ! x x x 4 x  ! attension handle this
  ! 9 x x 3 5
  !
  call AConstr(A, 5, 7)
  call Aassign(A, 1, (/1, 4/), (/1., 5./), 2)
  call Aassign(A, 2, (/2/), (/2./), 1)
  call Aassign(A, 3, (/1, 3, 4/), (/1., 2., 4./), 3)
  call Aassign(A, 4, (/4/), (/4./), 1)
  call Aassign(A, 5, (/1, 4, 5/), (/9.0, 3.0, 5./), 3)

  call Aprint(A)

  write(*,*) 'A%A: ', A%A
  write(*,*) 'A%JA:', A%JA
  write(*,*) 'A%IA:', A%IA

  print *, 'Test AIXDOT:'
  x = 1.0
  Do I = 1, A%N
     print *, AIXDOT(A, I, x)
  END DO

  print *, 'Test SPMV:'
  call spmv(1.0, A, x, 0.0 , b)
  do i = 1, n
     print *, b(i)
  end do

  max_it = 100
  flag = 0
  tol = 1.0e-15
  print *, 'reset x'
  x = 0.0
  call BiCGSTAB( n, A, x, b, max_it, tol, flag) 
  print *, 'After ', max_it, 'steps BiCGSTAB iteration'
  print *, x


contains

!========================================================
  subroutine AConstr(A, N, NNZ)
!========================================================
    ! Purpose:
    !
    implicit none

    type(sm) :: A
    integer :: N                          ! dimension of the A
    integer :: NNZ                        ! number of real non-zero elem

    A%N = N
    A%NNZ = NNZ
    allocate(A%A(NNZ) )
    allocate(A%JA(NNZ) )
    allocate(A%IA(N+1) )
    A%IA(1) = 1                           ! the first elme in first equation

  end subroutine AConstr

!========================================================
  subroutine Aassign(A, I, Js, Vals, N)
!========================================================
    ! Purpose: 
    ! Assign the ith row( ith equation) to A
    implicit none
    !
    type(sm) :: A
    integer  :: I                          ! ith Row
    integer  :: Js(*)                      ! J indexs 
    real     :: Vals(*)                    ! non-zero Elems
    integer  :: N                          ! number of non-zero Elem in ith Row
    
    !
    ! Local variables
    integer  :: IZ                         ! running index
    integer  :: IN                         ! running index

    ! ith equation alreay kown its starting id in A: IA(I)
    ! it has to assign the last index IA(I+1) 
    A%IA(I+1) = A%IA(I) + N

    IF (N .EQ. 0) Return

    DO IN = 1, N
       IZ = A%IA(I)+ IN -1 
       A%JA(IZ) = Js(IN)
       A%A(IZ)  = Vals(IN)
    END DO

  end subroutine Aassign

!========================================================
  subroutine Aprint(A)
!========================================================
    ! Purpose: 
    ! print the matrix A
    type(sm) :: A
    !Local variables
    integer :: I,J
    integer, pointer :: markers(:)
    integer :: IN

    ALLOCATE(markers(A%N))
    markers = -1

    DO I = 1, A%N

       DO IN = A%IA(I), A%IA(I+1)-1
          J = A%JA(IN)
          markers(J) = IN
       END DO
       
       DO J = 1, A%N
          IF (markers(J) /= -1) THEN
             WRITE(*,'(F6.2)',advance = 'no') A%A(markers(J))
          ELSE 
             WRITE(*,'(A6)', advance = 'no') 'X'
          END IF
       END DO

       WRITE(*,*) ' '
       
       ! Restore the values
       DO IN = A%IA(I), A%IA(I+1)-1
          J = A%JA(IN)
          markers(J) = -1
       END DO

    END DO
        
  end subroutine Aprint


!========================================================
  Function AIXDOT(A, i, x)
!========================================================
    ! Purpose:
    ! Dot product of the ith Row of A and x
    Implicit none
    !
    real :: AIXDOT
    type(sm) :: A
    integer  :: i  
    real :: x(*)

    ! Local variables
    integer :: IN
    !integer :: J

    AIXDOT = 0.0
    DO IN = A%IA(I), A%IA(I+1) -1 
       AIXDOT = AIXDOT + A%A(IN) * X(A%JA(IN))
    END DO
    
  END Function AIXDOT

!========================================================
  Subroutine SPMV(alpha , A, x, beta, y)
!========================================================
    ! Purpose: 
    ! spare version of the xGEMV in  BLAS
    ! y <-- alpha A x + beta y
    Implicit none
    !
    real,intent(in)  :: alpha, beta
    type(sm) :: A
    real, intent(in) :: x(*)
    real, intent(inout) :: y(*)


    ! Local variables
    integer :: I

    if (alpha == 1.0) then
       Do I = 1, A%N
          y(I) = AIXDOT(A, I, x) + beta * y(I)
       END DO
    else
       Do I = 1, A%N
          y(I) = AIXDOT(A, I, x) * alpha + beta * y(I)
       END DO
    END if

  END Subroutine SPMV


  !==========================
  Subroutine BiCGSTAB(n,A,x,b,max_it, tol, flag) 
    !==========================
    ! translated from the code matlab bicgstab.m 
    ! from templates
    implicit none
    !  [x, error, iter, flag] = bicgstab(A, x, b, M, max_it, tol)
    !
    ! bicgstab.m solves the linear system Ax=b using the 
    ! BiConjugate Gradient Stabilized Method with preconditioning.
    !
    ! input   A        REAL matrix
    !         x        REAL initial guess vector
    !         b        REAL right hand side vector
    !         M        REAL preconditioner matrix
    !         max_it   INTEGER maximum number of iterations
    !         tol      REAL error tolerance
    !
    ! output  x        REAL solution vector
    !         error    REAL error norm
    !         iter     INTEGER number of iterations performed
    !         flag     INTEGER: 0 = solution found to tolerance
    !                           1 = no convergence given max_it
    !                          -1 = breakdown: rho = 0
    !                          -2 = breakdown: omega = 0
    ! Parameter List
    integer, intent(in)  :: n              ! Dimension of matrix
    type(sm), intent(in) :: A
    real, dimension(n)   :: x, b
    integer,intent(inout) :: max_it 
    real,intent(in) :: tol
    integer,intent(out) :: flag
    
    ! Local variable
    real, dimension(n)   :: r,p, p_hat,r_tld,s ,s_hat,t,v
    real :: rho, rho_1, alpha, beta, bnrm2, error , omega, resid, snorm
    integer:: iter

    ! function from BLAS
    real :: snrm2
    real :: sdot

    ! initialization
    iter = 0
    flag = 0
    bnrm2 = snrm2(n, b, 1)
    if (bnrm2 == 0.0) bnrm2 = 1.0

    r = b
    !call sgemv('No', n, n, -1.0, A, n, x, 1, 1.0, r, 1) ! r = b - A x
    call spmv(-1.0, A, x, 1.0, r) ! r = b - A x
    
    error = snrm2(n, r, 1) / bnrm2 ! error = norm(r) /bnrm2
    if(error < tol) then
       print *, 'error < tol ==> return'
       return
    end if

    omega = 1.0
    r_tld = r

    iterLoop:do iter = 1, max_it 
       rho = sdot(n, r_tld, 1, r, 1)
       if (rho == 0.0) then
          print *, 'rho == 0.0 method fails ==> exit loop'
          exit
       end if
       if(iter > 1) then
          beta = (rho/rho_1) * (alpha / omega)
          p = r + beta * (p - omega * v)
       else
          p = r
       end if

       ! if M = I, p_hat = p
       p_hat = p
       !call sgemv('No', n, n, 1.0, A, n, p_hat, 1, 0.0, v, 1) !v = A * p_hat
       call spmv(1.0, A, p_hat, 0.0, v) !v = A * p_hat
       alpha = rho / sdot(n, r_tld, 1, v, 1)
       s = r - alpha * v
       snorm = snrm2(n,s,1)
       if( snorm < tol) then
          x = x + alpha * p_hat
          resid = snorm / bnrm2
          exit
       end if

       s_hat = s !if preconditioner M for s_hat = I
       !call sgemv('No', n, n, 1.0, A, n, s_hat, 1, 0.0, t, 1) !t = A * s_hat
       call spmv(1.0, A, s_hat, 0.0, t) !t = A * s_hat
       omega = sdot(n, t, 1, s, 1) / sdot(n, t, 1, t, 1) ! omega = t*s /t*t
       x = x +  alpha * p_hat + omega * s_hat            !update approximation
       r = s - omega * t
       error = snrm2(n, r, 1) / bnrm2 ! check convergence

       if (error <= tol) then
          print *, 'error <= tol converge end!'
          max_it = iter
          return
       end if
       if (omega == 0.0) then
          print *, 'omega == 0.0 exit'
          return
       end if

       rho_1 = rho
    end do iterLoop


    ! if ((error <= tol) .or. (s <= tol)) then         !converged
    !    if (s <= tol) then
    !       error = snrm2(n, s, 1) / bnrm2
    !    end if
    !    flag = 0
    ! else if (omega == 0.0) then                     ! breakdown
    !    flag = -2
    ! else if (rho == 0.0) then
    !    flag = -1
    ! else                  ! no convergence
    !    flag = 1
    ! end if


  end subroutine BiCGSTAB


end program ssms




