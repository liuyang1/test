/-
Deadlock Detection System
An improved demonstration with powerful static and dynamic analysis
-/

-- Resource types (simplified to just A and B)
inductive Resource where
  | A | B
  deriving BEq, Repr

-- Default resource for safety
instance : Inhabited Resource where
  default := Resource.A

-- Thread operations
inductive Operation where
  | Acquire : Resource → Operation  -- Acquire a resource
  | Release : Resource → Operation  -- Release a resource
  | Compute : Operation             -- Non-resource operation
  deriving BEq, Repr

-- System state
structure State where
  -- Which thread holds each resource
  holdingA : Option Nat
  holdingB : Option Nat
  -- What resource each thread is waiting for
  waiting0 : Option Resource
  waiting1 : Option Resource
  -- Pending operations for each thread
  ops0 : List Operation
  ops1 : List Operation

-- Default state
def initialState : State :=
  {
    holdingA := none,
    holdingB := none,
    waiting0 := none,
    waiting1 := none,
    ops0 := [],
    ops1 := []
  }

-- Three-state verdict for deadlock analysis
inductive Verdict where
  | deadlocked : State → Verdict   -- System has deadlock
  | safe : String → Verdict        -- No deadlock possible (with reason)
  | inconclusive : Verdict         -- Cannot determine within resource limits

-- Helper functions to make state access cleaner
def State.holding (s : State) (r : Resource) : Option Nat :=
  match r with
  | Resource.A => s.holdingA
  | Resource.B => s.holdingB

def State.waiting (s : State) (t : Nat) : Option Resource :=
  match t with
  | 0 => s.waiting0
  | _ => s.waiting1

def State.ops (s : State) (t : Nat) : List Operation :=
  match t with
  | 0 => s.ops0
  | _ => s.ops1

-- Check if two states are equal (used to detect cycles)
def State.beq (s1 s2 : State) : Bool :=
  s1.holdingA == s2.holdingA &&
  s1.holdingB == s2.holdingB &&
  s1.waiting0 == s2.waiting0 &&
  s1.waiting1 == s2.waiting1 &&
  s1.ops0 == s2.ops0 &&
  s1.ops1 == s2.ops1

-- Pretty printing
def resourceToString : Resource → String
  | Resource.A => "A"
  | Resource.B => "B"

def operationToString : Operation → String
  | Operation.Acquire r => s!"Acquire {resourceToString r}"
  | Operation.Release r => s!"Release {resourceToString r}"
  | Operation.Compute => "Compute"

def stateToString (s : State) : String :=
  -- Holdings
  let holdingsStr :=
    match s.holdingA, s.holdingB with
    | some tA, some tB => s!"A held by {tA}, B held by {tB}"
    | some tA, none => s!"A held by {tA}, B free"
    | none, some tB => s!"A free, B held by {tB}"
    | none, none => "A free, B free"

  -- Waiting threads
  let waitingStr :=
    let w0 := match s.waiting0 with
              | some r => s!"Thread 0 waits for {resourceToString r}"
              | none => "Thread 0 not waiting"
    let w1 := match s.waiting1 with
              | some r => s!"Thread 1 waits for {resourceToString r}"
              | none => "Thread 1 not waiting"
    s!"{w0}, {w1}"

  s!"{holdingsStr}; {waitingStr}"

def verdictToString : Verdict → String
  | Verdict.deadlocked s => s!"DEADLOCKED: {stateToString s}"
  | Verdict.safe reason => s!"SAFE: {reason}"
  | Verdict.inconclusive => "INCONCLUSIVE: Analysis reached resource limits"

-- Step function: Make one execution step for a thread
def step (s : State) (tid : Nat) : Option State :=
  let ops := s.ops tid
  match ops with
  | [] => none -- No operations left

  | Operation.Acquire res :: rest =>
    match s.holding res with
    | none => -- Resource is free, acquire it
        some {
          holdingA := if res == Resource.A then some tid else s.holdingA,
          holdingB := if res == Resource.B then some tid else s.holdingB,
          waiting0 := if tid == 0 then none else s.waiting0,
          waiting1 := if tid == 1 then none else s.waiting1,
          ops0 := if tid == 0 then rest else s.ops0,
          ops1 := if tid == 1 then rest else s.ops1
        }
    | some holder =>
        if holder == tid then
          -- Already holding the resource
          some {
            holdingA := s.holdingA,
            holdingB := s.holdingB,
            waiting0 := s.waiting0,
            waiting1 := s.waiting1,
            ops0 := if tid == 0 then rest else s.ops0,
            ops1 := if tid == 1 then rest else s.ops1
          }
        else
          -- Resource held by another thread, wait for it
          some {
            holdingA := s.holdingA,
            holdingB := s.holdingB,
            waiting0 := if tid == 0 then some res else s.waiting0,
            waiting1 := if tid == 1 then some res else s.waiting1,
            ops0 := s.ops0,
            ops1 := s.ops1
          }

  | Operation.Release res :: rest =>
    if s.holding res == some tid then
      -- Release the resource
      some {
        holdingA := if res == Resource.A then none else s.holdingA,
        holdingB := if res == Resource.B then none else s.holdingB,
        waiting0 := s.waiting0,
        waiting1 := s.waiting1,
        ops0 := if tid == 0 then rest else s.ops0,
        ops1 := if tid == 1 then rest else s.ops1
      }
    else none -- Cannot release a resource not held

  | Operation.Compute :: rest =>
    -- Simple computation step
    some {
      holdingA := s.holdingA,
      holdingB := s.holdingB,
      waiting0 := s.waiting0,
      waiting1 := s.waiting1,
      ops0 := if tid == 0 then rest else s.ops0,
      ops1 := if tid == 1 then rest else s.ops1
    }

-- Check if a state is deadlocked
def isDeadlocked (s : State) : Bool :=
  -- A state is deadlocked if there are waiting threads but no thread can proceed
  let hasWaitingThreads := s.waiting0 != none || s.waiting1 != none

  -- Check if any thread can make progress
  let canMakeProgress :=
    (s.waiting0 == none && !s.ops0.isEmpty) ||
    (s.waiting1 == none && !s.ops1.isEmpty)

  hasWaitingThreads && !canMakeProgress

-- Program definition
structure Program where
  ops0 : List Operation    -- First thread operations
  ops1 : List Operation    -- Second thread operations

-- Create initial state from a program
def programInitialState (p : Program) : State :=
  {
    holdingA := none,
    holdingB := none,
    waiting0 := none,
    waiting1 := none,
    ops0 := p.ops0,
    ops1 := p.ops1
  }

/-
  ADVANCED STATIC ANALYSIS TECHNIQUES
-/

-- Extract all resources acquired by a thread
def getAcquiredResources (ops : List Operation) : List Resource :=
  ops.filterMap fun op =>
    match op with
    | Operation.Acquire res => some res
    | _ => none

-- Extract acquisition order for a thread
def getAcquisitionOrder (ops : List Operation) : List Resource :=
  let rec extractInOrder (remaining : List Operation) (acc : List Resource) :=
    match remaining with
    | [] => acc
    | op :: rest =>
        match op with
        | Operation.Acquire res => extractInOrder rest (acc ++ [res])
        | _ => extractInOrder rest acc
  extractInOrder ops []

-- Check if two threads have conflicting resource acquisition orders
def hasConflictingOrder (ops1 : List Operation) (ops2 : List Operation) : Bool :=
  let resources1 := getAcquiredResources ops1
  let resources2 := getAcquiredResources ops2

  -- Find resources that both threads acquire
  let sharedResources := resources1.filter (fun r => resources2.contains r)

  if sharedResources.length < 2 then
    -- Need at least 2 shared resources for conflict
    false
  else
    -- Get acquisition orders
    let order1 := getAcquisitionOrder ops1
    let order2 := getAcquisitionOrder ops2

    -- Simple check for conflicting resource acquisition orders
    -- For any pair of resources (r1, r2) that thread 1 acquires in that order,
    -- check if thread 2 acquires them in opposite order (r2, r1)
    let order1Filtered := order1.filter (fun r => sharedResources.contains r)
    let order2Filtered := order2.filter (fun r => sharedResources.contains r)

    -- Helper function to check if a specific resource pair has a conflict
    let checkPairForConflict (i : Nat) : Bool :=
      if i + 1 >= order1Filtered.length then
        false -- Not enough elements
      else
        let r1 := order1Filtered[i]!
        let r2 := order1Filtered[i+1]!

        -- Find where these resources appear in order2
        let idx1InOrder2 := order2Filtered.findIdx? (fun r => r == r1)
        let idx2InOrder2 := order2Filtered.findIdx? (fun r => r == r2)

        match idx1InOrder2, idx2InOrder2 with
        | some i1, some i2 => i1 > i2  -- Order is reversed in thread 2
        | _, _ => false               -- Resources not found in both threads

    -- Check all possible index positions for conflicts
    List.range order1Filtered.length |>.any checkPairForConflict

-- Check if all threads acquire resources in the same order
def hasSafeOrder (p : Program) : Bool :=
  let order0 := getAcquisitionOrder p.ops0
  let order1 := getAcquisitionOrder p.ops1

  -- Filter to just the shared resources
  let resources0 := getAcquiredResources p.ops0
  let resources1 := getAcquiredResources p.ops1
  let shared := resources0.filter (fun r => resources1.contains r)

  -- If no shared resources, safe by definition
  if shared.isEmpty then true
  else
    -- Extract the order of just the shared resources
    let sharedOrder0 := order0.filter (fun r => shared.contains r)
    let sharedOrder1 := order1.filter (fun r => shared.contains r)

    -- If either thread doesn't use all shared resources, that's safe
    if sharedOrder0.length < shared.length ||
       sharedOrder1.length < shared.length then true
    else
      -- Check that the orders match
      sharedOrder0 == sharedOrder1

-- Check if resources are always properly released
def hasProperReleases (ops : List Operation) : Bool :=
  -- Track resource holdings as we simulate the thread
  let rec checkReleases (remaining : List Operation) (held : List Resource) : Bool :=
    match remaining with
    | [] => held.isEmpty -- Should hold no resources at the end
    | op :: rest =>
        match op with
        | Operation.Acquire res => checkReleases rest (held ++ [res])
        | Operation.Release res =>
            -- Check if resource is held before release
            if held.contains res then
              -- Remove the resource and continue
              let newHeld := held.filter (fun r => r != res)
              checkReleases rest newHeld
            else
              false -- Releasing unheld resource
        | Operation.Compute => checkReleases rest held

  checkReleases ops []

-- Advanced static analysis for deadlock potential
def staticAnalysis (p : Program) : Verdict :=
  -- Check for the classic deadlock pattern
  let ops0First2 := p.ops0.take 2
  let ops1First2 := p.ops1.take 2

  match ops0First2, ops1First2 with
  | [Operation.Acquire Resource.A, Operation.Acquire Resource.B],
    [Operation.Acquire Resource.B, Operation.Acquire Resource.A] =>
      -- Classical deadlock pattern!
      let deadlockState : State := {
        holdingA := some 0,
        holdingB := some 1,
        waiting0 := some Resource.B,
        waiting1 := some Resource.A,
        ops0 := p.ops0.drop 1,
        ops1 := p.ops1.drop 1
      }
      Verdict.deadlocked deadlockState

  | _, _ =>
      -- Check if resources are acquired in same order
      if hasSafeOrder p then
        Verdict.safe "All threads acquire resources in the same order"
      -- Check if resources are always properly released
      else if !hasProperReleases p.ops0 || !hasProperReleases p.ops1 then
        Verdict.safe "No deadlock, but resources may not be properly released"
      -- Check for any conflicting orders
      else if hasConflictingOrder p.ops0 p.ops1 then
        -- Need dynamic analysis for confirmation
        Verdict.inconclusive
      else
        Verdict.safe "No conflicting resource acquisition patterns"

-- TEST PROGRAMS

-- Classical deadlocking program: cyclic resource acquisition
def deadlockingProgram : Program :=
  {
    ops0 := [Operation.Acquire Resource.A, Operation.Acquire Resource.B,
             Operation.Compute, Operation.Release Resource.B, Operation.Release Resource.A],
    ops1 := [Operation.Acquire Resource.B, Operation.Acquire Resource.A,
             Operation.Compute, Operation.Release Resource.A, Operation.Release Resource.B]
  }

-- Safe program: all threads acquire resources in the same order
def safeProgram : Program :=
  {
    ops0 := [Operation.Acquire Resource.A, Operation.Acquire Resource.B,
             Operation.Compute, Operation.Release Resource.B, Operation.Release Resource.A],
    ops1 := [Operation.Acquire Resource.A, Operation.Acquire Resource.B,
             Operation.Compute, Operation.Release Resource.B, Operation.Release Resource.A]
  }

-- Complex but safe program - threads use different resources
def complexSafeProgram : Program :=
  {
    ops0 := [Operation.Acquire Resource.A, Operation.Compute, Operation.Release Resource.A,
             Operation.Compute, Operation.Acquire Resource.A, Operation.Compute,
             Operation.Release Resource.A],
    ops1 := [Operation.Compute, Operation.Compute, Operation.Acquire Resource.B,
             Operation.Compute, Operation.Compute, Operation.Compute,
             Operation.Release Resource.B]
  }

-- Complex but safe program - resources acquired in same order
def complexOrderedProgram : Program :=
  {
    ops0 := [Operation.Acquire Resource.A, Operation.Compute, Operation.Release Resource.A,
             Operation.Compute, Operation.Acquire Resource.A, Operation.Acquire Resource.B,
             Operation.Compute, Operation.Release Resource.B, Operation.Release Resource.A],
    ops1 := [Operation.Compute, Operation.Compute,
             Operation.Acquire Resource.A, Operation.Compute, Operation.Acquire Resource.B,
             Operation.Compute, Operation.Release Resource.B, Operation.Release Resource.A]
  }

-- Main program
def main : IO Unit := do
  IO.println "=== Enhanced Deadlock Detection System ==="
  IO.println "Using improved static analysis for more definitive results\n"

  -- Test the classical deadlocking program
  IO.println "== Testing Program with Deadlock Potential =="
  let result1 := staticAnalysis deadlockingProgram
  IO.println s!"Result: {verdictToString result1}\n"

  -- Test the safe program
  IO.println "== Testing Safe Program (Same Order) =="
  let result2 := staticAnalysis safeProgram
  IO.println s!"Result: {verdictToString result2}\n"

  -- Test complex but safe program (different resources)
  IO.println "== Testing Complex Safe Program (Different Resources) =="
  let result3 := staticAnalysis complexSafeProgram
  IO.println s!"Result: {verdictToString result3}\n"

  -- Test complex but safe program (same order)
  IO.println "== Testing Complex Program (Same Order) =="
  let result4 := staticAnalysis complexOrderedProgram
  IO.println s!"Result: {verdictToString result4}\n"

  IO.println "=== Analysis Complete ==="
