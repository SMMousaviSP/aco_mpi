using Plots
using CSV
using DataFrames
using Statistics

df::DataFrame = CSV.read("../env/result.csv", DataFrame)


plot(df[!, "iteration"], df[!, "pathLength"])
savefig("env/plot.png")

gdf = groupby(df, :iteration)
combined = combine(gdf, 
                   :pathLength => mean => :mean, 
                   :pathLength => std => :std,
                   :pathLength => minimum => :best)

println(combined)

plot(combined[!, "iteration"], combined[!, "mean"], ribbon=combined[!, "std"])
savefig("env/plot_mean.png")

plot(combined[!, "iteration"], combined[!, "best"])
savefig("env/plot_best.png")
