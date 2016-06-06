bin_PROGRAMS = clasp_w

clasp_w_CXXFLAGS = -I ../libclasp
clasp_w_LDFLAGS = -L ../libclasp
if !IS_DARWIN
clasp_w_LDFLAGS += -static
endif # IS_DARWIN

clasp_w_LDADD = -lclasp -lpthread

clasp_w_SOURCES = \
clasp_solver.cpp \
clock.cpp \
criterion.cpp \
criterion_exp.cpp \
criterion_window.cpp \
main.cpp \
stopper_2c.cpp \
stopper.cpp \
stopper_window.cpp


noinst_HEADERS = \
clasp_solver.h \
clock.h \
criterion_exp.h \
criterion.h \
criterion_window.h \
stopper_2c.h \
stopper.h \
stopper_window.h

