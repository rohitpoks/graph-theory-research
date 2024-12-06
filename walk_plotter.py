import pandas
from plotnine import *

data = pandas.read_csv("walk_plotter.csv")

K_1_2 = data[data["original_graph"] == "K_1_2"]
K_2_3 = data[data["original_graph"] == "K_2_3"]
modified_K_2_3 = data[data["original_graph"] == "modified_K_2_3"]
K_2_3_combined = pandas.concat([K_2_3, modified_K_2_3], ignore_index="True")
modified_K4 = data[data["original_graph"] == "modified_K4"]

line_chart_1 = (
    ggplot(K_1_2)
        + aes(x="k", y="steps")
        + geom_line()
        + ggtitle("avg number of steps for K_1_2")
)

line_chart_2 = (
    ggplot(K_2_3)
        + aes(x="k", y="steps")
        + geom_line()
        + ggtitle("avg number of steps for K_2_3")
)

line_chart_3 = (
    ggplot(modified_K_2_3)
        + aes(x="k", y="steps")
        + geom_line()
        + ggtitle("avg number of steps for modified_K_2_3")
)

line_chart_combined = (
    ggplot(K_2_3_combined)
        + aes(x="k", y="steps", color="original_graph")
        + geom_line()
        + ggtitle("avg number of steps for each coloring graph")
)

line_chart_4 = (
     ggplot(modified_K4)
        + aes(x="k", y="steps")
        + geom_line()
        + ggtitle("avg number of steps for modified_K4")
)

# bar_chart = (
#     ggplot(data)
#         + aes(x="original_graph", y="steps", fill="factor(k)")
#         + geom_col(position="dodge")
#         + ggtitle("another test!")
# )

# line_chart_1.save("walk_line_chart_1.png")
# line_chart_2.save("walk_line_chart_2.png")
# line_chart_3.save("walk_line_chart_3.png")
# line_chart_combined.save("walk_line_chart_combined.png")
line_chart_4.save("walk_line_chart_4.png")