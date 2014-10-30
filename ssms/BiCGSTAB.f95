!==========================
Subroutine BiCGSTAB()
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
real, dimension(n,n) :: A(n,n)
real, dimension(n)   :: x, b
real, dimension(n)   :: x0, b,r,p, p_hat,r_tld,s ,s_hat,t,v
real :: rho, rho_1, tol, alpha, beta, bnrm2, error , omega, resid, snorm
integer:: i
!integer :: fid = 10
integer :: flag, iter, max_it


! function from BLAS
real :: snrm2
real :: sdot

!================  preparation of A x = b ==============
!open(unit = fid, file = 'A.mat')
!read(fid, *) A


CALL random_seed()
call random_number(A)
! do i = 1, n
!    A(i,i) = 10 * real(i)
! end do

x = (/(real(i), i = 1,n)/)

print *, '====== A x = b ======'
print *, 'A:'
do i = 1, n
   write(*, *) A(i,:)
end do
print *, 'x:'
do i = 1, n
   write(*, *) x(i)
end do

call sgemv('No', n, n, 1.0, A, n, x, 1, 0.0, b, 1) ! b = A x
print *, 'b:'
do i = 1, n
   write(*, *) b(i)
end do
x0 = x
print *, 'set x to 0'
x = 0.0

! ============= set parameters ==============================
tol = 1.0e-14
max_it = 100
print *, 'tol =', tol, 'max_it', max_it


! initialization
iter = 0
flag = 0
bnrm2 = snrm2(n, b, 1)
if (bnrm2 == 0.0) bnrm2 = 1.0


r = b
call sgemv('No', n, n, -1.0, A, n, x, 1, 1.0, r, 1) ! r = b - A x
error = snrm2(n, r, 1) / bnrm2 ! error = norm(r) /bnrm2
if(error < tol) then
   print *, 'error < tol ==> exit'
   stop
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
   call sgemv('No', n, n, 1.0, A, n, p_hat, 1, 0.0, v, 1) !v = A * p_hat
   alpha = rho / sdot(n, r_tld, 1, v, 1)
   s = r - alpha * v
   snorm = snrm2(n,s,1)
   if( snorm < tol) then
      x = x + alpha * p_hat
      resid = snorm / bnrm2
      exit
   end if
   
   s_hat = s !if preconditioner M for s_hat = I
   call sgemv('No', n, n, 1.0, A, n, s_hat, 1, 0.0, t, 1) !t = A * s_hat
   omega = sdot(n, t, 1, s, 1) / sdot(n, t, 1, t, 1) ! omega = t*s /t*t
   x = x +  alpha * p_hat + omega * s_hat            !update approximation
   r = s - omega * t
   error = snrm2(n, r, 1) / bnrm2 ! check convergence
   
   if (error <= tol) then
      print *, 'error <= tol end!'
      exit
   end if
   if (omega == 0.0) then
      print *, 'omega == 0.0 exit'
      exit
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


print *, 'after',iter, ' iteration'
write(*, '(3A12)') 'Result', 'Actual', 'Error %'
do i = 1, n
   write(*, '(3E12.4)') x(i), x0(i), (x(i) - x0(i))/ x0(i) * 100
end do



end subroutine BiCGSTAB
