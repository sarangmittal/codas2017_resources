# include "MPlexTest.h"

const int MPlexTest::sNMul = MPT_DIM*MPT_DIM*(2*MPT_DIM - 1) * MPT_SIZE;

//==============================================================================

MPlexTest::MPlexTest(int n_mp, int n_mp_sym, int size) :
  fN(n_mp), fNS(n_mp_sym)
{
  fMPV = new MPV*[fN];

  for (int i = 0; i < fN; ++i)
  {
    fMPV[i] = new MPV(size);
  }

  fMPVS = new MPVS*[fNS];

  for (int i = 0; i < fNS; ++i)
  {
    fMPVS[i] = new MPVS(size);
  }
}

MPlexTest::~MPlexTest()
{
  for (int i = 0; i < fNS; ++i)
  {
    delete fMPVS[i];
  }
  delete fMPVS;

  for (int i = 0; i < fN; ++i)
  {
    delete fMPV[i];
  }
  delete fMPV;
}

//==============================================================================

Func_t MPlexTest::name_to_func(const std::string& name)
{
  // perl -ne 'if (m/^\s*long64\s+(\w[\w\d]*)\(int\s+\w+\);\s*$/) { print "  if (name.compare(\"$1\") == 0) return [&](int n) { return $1(n); };\n"; }' MPlexTest.h

  if (name.compare("mult2") == 0) return [&](int n) { return mult2(n); };
  if (name.compare("mult2_general") == 0) return [&](int n) { return mult2_general(n); };
  if (name.compare("mult2_3out") == 0) return [&](int n) { return mult2_3out(n); };
  if (name.compare("inv_cramer") == 0) return [&](int n) { return inv_cramer(n); };
  if (name.compare("inv_cholesky") == 0) return [&](int n) { return inv_cholesky(n); };
  if (name.compare("mult2_sym") == 0) return [&](int n) { return mult2_sym(n); };
  if (name.compare("mult2_sym_general") == 0) return [&](int n) { return mult2_sym_general(n); };
  if (name.compare("inv_cramer_sym") == 0) return [&](int n) { return inv_cramer_sym(n); };
  if (name.compare("inv_cholesky_sym") == 0) return [&](int n) { return inv_cholesky_sym(n); };

  throw std::runtime_error("MPlexTest::name_to_func no match");
}

//==============================================================================

long64 MPlexTest::mult2(int n_vec)
{
  Matriplex::Multiply(*fMPV[0], *fMPV[1], *fMPV[2], n_vec);

  return sNMul * n_vec;
}

long64 MPlexTest::mult2_general(int n_vec)
{
  Matriplex::MultiplyGeneral(*fMPV[0], *fMPV[1], *fMPV[2], n_vec);

  return sNMul * n_vec;
}

long64 MPlexTest::mult2_3out(int n_vec)
{
  Matriplex::Multiply(*fMPV[0], *fMPV[1], *fMPV[2], n_vec);
  Matriplex::Multiply(*fMPV[1], *fMPV[2], *fMPV[0], n_vec);
  Matriplex::Multiply(*fMPV[2], *fMPV[0], *fMPV[1], n_vec);

  return 3 * sNMul * n_vec;
}

long64 MPlexTest::mult2_3in(int n_vec)
{
  Matriplex::Multiply3in(*fMPV[0], *fMPV[1], *fMPV[2], n_vec);

  return 3 * sNMul * n_vec;
}

//==============================================================================

long64 MPlexTest::inv_cramer(int n_vec)
{
  assert(MPT_DIM == 3);

  Matriplex::InvertCramer(*fMPV[0], n_vec);

  // 3x3: 27 + 5 + 1 + 9 = 42 
  return 42 * MPT_SIZE * n_vec;
}

long64 MPlexTest::inv_cholesky(int n_vec)
{
  assert(MPT_DIM == 3);

  Matriplex::InvertCholesky(*fMPV[0], n_vec);

  // 3x3: 32 ops + 3 sqrts
  // sqrt: latency 8, throughput 2 (have to wait on each of them!)
  // what do i do then? count each as 4 (every other clock!).
  return 44 * MPT_SIZE * n_vec;
}


//==============================================================================
// Symmetric
//==============================================================================

long64 MPlexTest::mult2_sym(int n_vec)
{
  Matriplex::Multiply(*fMPVS[0], *fMPVS[1], *fMPV[0], n_vec);

  return sNMul * n_vec;
}

long64 MPlexTest::mult2_sym_general(int n_vec)
{
  Matriplex::MultiplyGeneral(*fMPVS[0], *fMPVS[1], *fMPV[0], n_vec);

  return sNMul * n_vec;
}

long64 MPlexTest::inv_cramer_sym(int n_vec)
{
  assert(MPT_DIM == 3);

  Matriplex::InvertCramerSym(*fMPVS[0], n_vec);

  // 3x3: 18 + 5 + 1 + 6 = 30 
  return 30 * MPT_SIZE * n_vec;
}

long64 MPlexTest::inv_cholesky_sym(int n_vec)
{
  assert(MPT_DIM == 3);

  Matriplex::InvertCholeskySym(*fMPVS[0], n_vec);

  // 3x3: 32 ops + 3 sqrts
  // sqrt: latency 8, throughput 2 (have to wait on each of them!)
  // what do i do then? count each as 4 (every other clock!).
  return 44 * MPT_SIZE * n_vec;
}
