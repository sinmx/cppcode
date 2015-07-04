(module sec2.2-proc-rep (lib "eopl.ss" "eopl")

  ;; Simple procedural representation of environments
  ;; Page: 40

  (require "utils.scm")

  ;; data definition:
  ;; Env = Var -> Schemeval

  ;; empty-env : () -> Env
  (define empty-env
    (lambda ()
      (list
       (lambda (search-var) 
         (report-no-binding-found search-var))
       (lambda () #t)
       (lambda (search-var)
         (report-no-binding-found search-var)))))
  
  ;; extend-env : Var * Schemeval * Env -> Env
  (define extend-env
    (lambda (saved-var saved-val saved-env)
      (list
       (lambda (search-var)
         (if (eqv? search-var saved-var)
             saved-val
             (apply-env saved-env search-var)))
       (lambda () #f)
       (lambda (search-var)
         (if (eqv? search-var saved-var)
             #t
             ((env-hasbinding saved-env) search-var))))))
  (define env-search car)
  (define env-isempty cadr)
  (define env-hasbinding caddr)
  ;; apply-env : Env * Var -> Schemeval
  (define apply-env
    (lambda (env search-var) 
      ((env-search env) search-var)))
  (define env-isempty?
    (lambda (env)
      ((env-isempty env))))
  (define env-hasbinding?
    (lambda (env search-var) 
      ((env-hasbinding env) search-var)))
  (define report-no-binding-found
    (lambda (search-var)
      (eopl:error 'apply-env "No binding for ~s" search-var)))
  
  (define report-invalid-env
    (lambda (env)
      (eopl:error 'apply-env "Bad environment: ~s" env)))

  (define e
    (extend-env 'd 6
      (extend-env 'y 8
        (extend-env 'x 7
          (extend-env 'y 14
            (empty-env))))))

  (equal?? (apply-env e 'd) 6)
  (equal?? (apply-env e 'y) 8)
  (equal?? (apply-env e 'x) 7)
  (equal?? (env-isempty? e) #f)
  (equal?? (env-isempty? (empty-env)) #t)
  (equal?? (env-hasbinding? e 'x) #t)
  (report-unit-tests-completed 'apply-env)
  )


