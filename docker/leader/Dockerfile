FROM satcomp-light:common AS builder
USER root

FROM satcomp-infrastructure:leader AS mallob_liaison
WORKDIR /

COPY --from=builder /cloud-sat/light light

COPY --chown=ecs-user /init_solver.sh /competition/init_solver.sh
COPY --chown=ecs-user /run_solver.sh /competition/run_solver.sh
COPY --chown=ecs-user /solver /competition/solver

USER ecs-user
RUN chmod +x /competition/init_solver.sh
RUN chmod +x /competition/run_solver.sh
RUN chmod +x /competition/solver
