struct Drawable {
  bool hidden;
  int drawcount;
};

struct Position {
  float x;
  float y;
  float z;
};

struct Velocity {
  float dx;
  float dy;
  float dz;
};

struct Acceleration {
  double ax;
  double ay;
  double az;
};

static constexpr unsigned long long componentCount = 100000;

static void ABABTest(benchmark::State& state) {

  struct PV {
    Drawable drawable;
    Position pos;
    Velocity vel;
    Acceleration acc;
  };

  PV* data = new PV[componentCount];

  for (auto _ : state) {
    /*PV* itend = data + componentCount;
    for (PV* it = data; it < itend; ++it) {
      it->pos.x += it->vel.dx;
      it->pos.y += it->vel.dy;
      it->pos.z += it->vel.dz;
    }*/
    for (int i = 0; i < componentCount; ++i) {
      if (!data[i].drawable.hidden) {
        data[i].drawable.drawcount++;
      }
      data[i].pos.x += data[i].vel.dx;
      data[i].pos.y += data[i].vel.dy;
      data[i].pos.z += data[i].vel.dz;
      data[i].vel.dx /= 1.3;
      data[i].vel.dy /= 1.3;
      data[i].vel.dz /= 1.3;
      data[i].vel.dx += data[i].vel.dx * data[i].acc.ax;
      data[i].vel.dy += data[i].vel.dy * data[i].acc.ay;
      data[i].vel.dz += data[i].vel.dz * data[i].acc.az;
    }
  }

  benchmark::DoNotOptimize(data);

  delete[] data;
}

BENCHMARK(ABABTest);

static void AABBTest(benchmark::State& state) {
  Drawable* drawable = new Drawable[componentCount];
  Position* positions = new Position[componentCount];
  Velocity* velocity = new Velocity[componentCount];
  Acceleration* acceleration = new Acceleration[componentCount];

  for (auto _ : state) {
    for (int i = 0; i < componentCount; ++i) {
      if (drawable[i].hidden) {
        drawable[i].drawcount++;
      }
      positions[i].x += velocity[i].dx;
      positions[i].y += velocity[i].dy;
      positions[i].z += velocity[i].dz;
      velocity[i].dx /= 1.3;
      velocity[i].dy /= 1.3;
      velocity[i].dz /= 1.3;
      velocity[i].dx += velocity[i].dx * acceleration[i].ax;
      velocity[i].dy += velocity[i].dy * acceleration[i].ay;
      velocity[i].dz += velocity[i].dz * acceleration[i].az;
    }
  }

  benchmark::DoNotOptimize(drawable);
  benchmark::DoNotOptimize(positions);
  benchmark::DoNotOptimize(velocity);
  benchmark::DoNotOptimize(acceleration);

  delete[] drawable;
  delete[] positions;
  delete[] velocity;
  delete[] acceleration;
}

BENCHMARK(AABBTest);